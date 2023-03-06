// based on https://stackoverflow.com/questions/37572961/c-directwrite-load-font-from-file-at-runtime

#pragma once

#include "framework.h"


class FontCollectionLoader : public IDWriteFontCollectionLoader
{
public:
	FontCollectionLoader();

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject) override;
	ULONG STDMETHODCALLTYPE AddRef() override;
	ULONG STDMETHODCALLTYPE Release() override;

	HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(IDWriteFactory* factory, void const* collectionKey, UINT32 collectionKeySize, IDWriteFontFileEnumerator** fontFileEnumerator) override;

	static IDWriteFontCollectionLoader* GetLoader();
	static bool IsLoaderInitialized();

private:
	ULONG refCount;
	static IDWriteFontCollectionLoader* instance;
};


class FontFileEnumerator : public IDWriteFontFileEnumerator
{
public:
	FontFileEnumerator(IDWriteFactory* DWriteFactory);
	~FontFileEnumerator();

	void Initialize(size_t collectionKey);

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject) override;
	ULONG STDMETHODCALLTYPE AddRef() override;
	ULONG STDMETHODCALLTYPE Release() override;

	HRESULT STDMETHODCALLTYPE MoveNext(BOOL* hasCurrentFile) override;
	HRESULT STDMETHODCALLTYPE GetCurrentFontFile(IDWriteFontFile** fontFile) override;


private:
	vector<wstring> filePaths;
	IDWriteFactory* DWriteFactory;
	IDWriteFontFile* currentFile;
	ULONG nextIndex;
	ULONG refCount;
};


class FontContext
{
public:
	FontContext(IDWriteFactory* DWriteFactory);
	~FontContext();

	void CreateFontCollection(const vector<wstring>& newCollection, IDWriteFontCollection** result);


private:
	IDWriteFactory* DWriteFactory;
	bool isInitialized;

	static vector<size_t> cKeys;
};
