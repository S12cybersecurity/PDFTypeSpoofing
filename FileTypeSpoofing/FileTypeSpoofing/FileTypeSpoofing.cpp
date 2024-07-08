#include <windows.h>
#include <shlobj.h>
#include <atlbase.h>
#include <string>
#include <iostream>

bool CreateShortcut(const std::wstring& targetPath, const std::wstring& shortcutPath, const std::wstring& iconPath)
{
    HRESULT hres;
    CComPtr<IShellLink> psl;

    CoInitialize(NULL);

    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres))
    {
        CComPtr<IPersistFile> ppf;

        psl->SetPath(targetPath.c_str());

        psl->SetIconLocation(iconPath.c_str(), 0);

        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
        if (SUCCEEDED(hres))
        {
            hres = ppf->Save(shortcutPath.c_str(), TRUE);
            ppf.Release();
        }
        psl.Release();
    }

    CoUninitialize();
    return SUCCEEDED(hres);
}

int main()
{
    std::wstring targetPath = L"C:\\Windows\\System32\\calc.exe";    
    std::wstring shortcutPath = L"C:\\Users\\b\\Desktop\\Calculator.lnk";
    // icon path its the same than this path, find the current path
    char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	std::string path = std::string(buffer).substr(0, pos);
	std::wstring iconPath = std::wstring(path.begin(), path.end()) + L"\\pdfIcon.ico";


    if (CreateShortcut(targetPath, shortcutPath, iconPath))
    {
		return 0;
    }
    else
    {
		return 1;
    }
}
