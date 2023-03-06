#include "FontLoader.h"


// Acquires an additional reference, if non-null.
template <typename InterfaceType>
inline InterfaceType* SafeAcquire(InterfaceType* newObject)
{
	if (newObject != nullptr)
		newObject->AddRef();
	return newObject;
}


vector<vector<wstring>> FontCollections;
IDWriteFontCollectionLoader* FontCollectionLoader::instance(DBG_NEW FontCollectionLoader());
vector<size_t> FontContext::cKeys;


FontCollectionLoader::FontCollectionLoader()
	: refCount(0)
{
}
HRESULT STDMETHODCALLTYPE FontCollectionLoader::QueryInterface(REFIID iid, void** ppvObject)
{
	if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontCollectionLoader))
	{
		*ppvObject = this;
		AddRef();
		return S_OK;
	}

	*ppvObject = nullptr;
	return E_NOINTERFACE;
}
ULONG STDMETHODCALLTYPE FontCollectionLoader::AddRef()
{
	return InterlockedIncrement(&refCount);
}
ULONG STDMETHODCALLTYPE FontCollectionLoader::Release()
{
	ULONG newCount = InterlockedDecrement(&refCount);
	if (newCount == 0) delete this;
	return newCount;
}
HRESULT STDMETHODCALLTYPE FontCollectionLoader::CreateEnumeratorFromKey(IDWriteFactory* factory, void const* collectionKey, UINT32 collectionKeySize, IDWriteFontFileEnumerator** fontFileEnumerator)
{
	*fontFileEnumerator = nullptr;

	if (collectionKeySize % sizeof(size_t) != 0)
		return E_INVALIDARG;

	FontFileEnumerator* enumerator = DBG_NEW FontFileEnumerator(factory);
	if (enumerator == nullptr)
		return E_OUTOFMEMORY;

	enumerator->Initialize(*(size_t*)collectionKey);
	*fontFileEnumerator = SafeAcquire(enumerator);

	return S_OK;
}
IDWriteFontCollectionLoader* FontCollectionLoader::GetLoader()
{
	return instance;
}
bool FontCollectionLoader::IsLoaderInitialized()
{
	return instance != nullptr;
}


FontFileEnumerator::FontFileEnumerator(IDWriteFactory* DWriteFactory)
	: DWriteFactory(SafeAcquire(DWriteFactory)), currentFile(nullptr), nextIndex(0), refCount(0)
{
}
FontFileEnumerator::~FontFileEnumerator()
{
	SafeRelease(&currentFile);
	SafeRelease(&DWriteFactory);
}
void FontFileEnumerator::Initialize(size_t collectionKey)
{
	for (const wstring& str : FontCollections[collectionKey])
		filePaths.push_back(str);
}
HRESULT STDMETHODCALLTYPE FontFileEnumerator::QueryInterface(REFIID iid, void** ppvObject)
{
	if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileEnumerator))
	{
		*ppvObject = this;
		AddRef();
		return S_OK;
	}
	*ppvObject = nullptr;
	return E_NOINTERFACE;
}
ULONG STDMETHODCALLTYPE FontFileEnumerator::AddRef()
{
	return InterlockedIncrement(&refCount);
}
ULONG STDMETHODCALLTYPE FontFileEnumerator::Release()
{
	ULONG newCount = InterlockedDecrement(&refCount);
	if (newCount == 0) delete this;
	return newCount;
}
HRESULT STDMETHODCALLTYPE FontFileEnumerator::MoveNext(BOOL* hasCurrentFile)
{
	HRESULT hr = S_OK;

	*hasCurrentFile = FALSE;
	SafeRelease(&currentFile);

	if (nextIndex < filePaths.size())
	{
		hr = DWriteFactory->CreateFontFileReference(filePaths[nextIndex].c_str(), nullptr, &currentFile);

		if (SUCCEEDED(hr))
		{
			*hasCurrentFile = TRUE;
			nextIndex += 1;
		}
	}

	return hr;
}
HRESULT STDMETHODCALLTYPE FontFileEnumerator::GetCurrentFontFile(IDWriteFontFile** fontFile)
{
	*fontFile = SafeAcquire(currentFile);
	return (currentFile != nullptr) ? S_OK : E_FAIL;
}


FontContext::FontContext(IDWriteFactory* DWriteFactory)
	: DWriteFactory(DWriteFactory), isInitialized(false)
{
}
FontContext::~FontContext()
{
	DWriteFactory->UnregisterFontCollectionLoader(FontCollectionLoader::GetLoader());
}
void FontContext::CreateFontCollection(const vector<wstring>& newCollection, IDWriteFontCollection** result)
{
	*result = nullptr;

	// save new collection in FontGlobals::fontCollections vector
	FontCollections.push_back(newCollection);
	cKeys.push_back(FontCollections.size()-1);
	
	if (!isInitialized && FontCollectionLoader::IsLoaderInitialized())
	{
		// Register our custom loader with the factory object.
		isInitialized = SUCCEEDED(DWriteFactory->RegisterFontCollectionLoader(FontCollectionLoader::GetLoader()));
	}

	DWriteFactory->CreateCustomFontCollection(FontCollectionLoader::GetLoader(), &cKeys.back(), sizeof(cKeys.back()), result);
}
