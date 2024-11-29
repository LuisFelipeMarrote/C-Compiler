#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <richedit.h>
#include "../sintatico.h"

#define ID_EDIT_ARQUIVO 1001
#define ID_EDIT_CODIGO 1002
#define ID_EDIT_ERROS 1003
#define ID_BTN_COMPILAR 1004
#define ID_STATUSBAR 1005
#define ID_FILE_OPEN 1006
#define ID_FILE_SAVE 1007 //salvar como
#define ID_FILE_SAVE2 1010 // salvar
#define ID_FILE_EXIT 1008
#define ID_HELP_ABOUT 1009
#define ID_FILE_CLOSE 1011

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void AddControls(HWND hwnd);
void AddMenus(HWND hwnd);
void AbrirArquivo(HWND hwnd);
void SalvarArquivo(HWND hwnd);
void SalvarArquivoOriginal(HWND hwnd);
void Compilacao(HWND hwnd);
void CloseFile(HWND hwnd);
HWND hArquivo, hCodigo, hErros, hStatus, hCompile;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"Compilador";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc))
        return -1;

    CreateWindowW(L"Compilador", L"Compilador",
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                 100, 100, 800, 600,
                 NULL, NULL, hInst, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

void AddMenus(HWND hwnd) {
    HMENU hMenuBar = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hHelpMenu = CreateMenu();

    // Menu Arquivo
    AppendMenuW(hFileMenu, MF_STRING, ID_FILE_OPEN, L"Abrir");
    AppendMenuW(hFileMenu, MF_STRING, ID_FILE_SAVE, L"Salvar Como");  // Renomeado para maior clareza
    AppendMenuW(hFileMenu, MF_STRING, ID_FILE_SAVE2, L"Salvar"); 
    AppendMenuW(hFileMenu, MF_STRING, ID_FILE_CLOSE, L"Fechar Arquivo"); 
    AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hFileMenu, MF_STRING, ID_FILE_EXIT, L"Sair");

    // Menu Ajuda
    AppendMenuW(hHelpMenu, MF_STRING, ID_HELP_ABOUT, L"Sobre");

    // Adiciona os menus à barra de menu
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"Arquivo");
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hHelpMenu, L"Ajuda");

    SetMenu(hwnd, hMenuBar);
}

void AbrirArquivo(HWND hwnd) {
    OPENFILENAMEW ofn;
    wchar_t fileName[100];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(fileName);
    ofn.lpstrFilter = L"Todos os arquivos\0*.*\0Arquivos de texto\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        HANDLE hFile = CreateFileW(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            DWORD fileSize = GetFileSize(hFile, NULL);
            if (fileSize != INVALID_FILE_SIZE) {
                char* buffer = (char*)malloc(fileSize + 1);
                DWORD bytesRead;
                if (ReadFile(hFile, buffer, fileSize, &bytesRead, NULL)) {
                    buffer[fileSize] = '\0';
                    SetWindowTextA(hCodigo, buffer);
                    SetWindowTextW(hArquivo, fileName);
                }
                free(buffer);
            }
            CloseHandle(hFile);
        }
    }
}

void SalvarArquivo(HWND hwnd) { //salvar como
    OPENFILENAMEW ofn;
    wchar_t fileName[100];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(fileName);
    ofn.lpstrFilter = L"Todos os arquivos\0*.*\0Arquivos de texto\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameW(&ofn)) {
        HANDLE hFile = CreateFileW(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            int length = GetWindowTextLengthA(hCodigo);
            if (length > 0) {
                char* buffer = (char*)malloc(length + 1);
                GetWindowTextA(hCodigo, buffer, length + 1);
                DWORD bytesWritten;
                WriteFile(hFile, buffer, length, &bytesWritten, NULL);
                free(buffer);

                // Atualiza o campo de arquivo com o novo nome
                SetWindowTextW(hArquivo, fileName);
                MessageBoxW(hwnd, L"Arquivo salvo com sucesso!", L"Sucesso", MB_OK | MB_ICONINFORMATION);
            }
            CloseHandle(hFile);
        } else {
            MessageBoxW(hwnd, L"Erro ao salvar o arquivo!", L"Erro", MB_OK | MB_ICONERROR);
        }
    }
}

void SalvarArquivoOriginal(HWND hwnd) {
    // Obtém o nome do arquivo atual da caixa de texto
    wchar_t fileName[100];
    GetWindowTextW(hArquivo, fileName, 100);

    // Verifica se existe um arquivo aberto
    if (wcslen(fileName) == 0) {
        MessageBoxW(hwnd, L"Nenhum arquivo aberto para salvar!", L"Erro", MB_OK | MB_ICONERROR);
        return;
    }

    // Abre o arquivo original para escrita
    HANDLE hFile = CreateFileW(fileName, GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        int length = GetWindowTextLengthA(hCodigo);
        if (length > 0) {
            char* buffer = (char*)malloc(length + 1);
            GetWindowTextA(hCodigo, buffer, length + 1);
            DWORD bytesWritten;
            WriteFile(hFile, buffer, length, &bytesWritten, NULL);
            free(buffer);
            MessageBoxW(hwnd, L"Arquivo salvo com sucesso!", L"Sucesso", MB_OK | MB_ICONINFORMATION);
        }
        CloseHandle(hFile);
    } else {
        MessageBoxW(hwnd, L"Erro ao salvar o arquivo!", L"Erro", MB_OK | MB_ICONERROR);
    }
}

void CloseFile(HWND hwnd){
    // Verificar se há alterações não salvas
    int length = GetWindowTextLengthA(hCodigo);
    if (length > 0) {
        int result = MessageBoxW(hwnd, 
            L"Deseja salvar as alterações antes de fechar o arquivo?", 
            L"Fechar Arquivo", 
            MB_YESNOCANCEL | MB_ICONQUESTION);
        
        switch(result) {
            case IDYES:
                // Tenta salvar o arquivo
                SalvarArquivo(hwnd);
                break;
            case IDCANCEL:
                // Cancela a operação de fechar
                return;
        }
    }
    
    // Limpa os campos de arquivo e código
    SetWindowTextW(hArquivo, L"");
    SetWindowTextA(hCodigo, "");
    SetWindowTextA(hErros, "");

}

void SetCursorToLine(HWND hEdit, int lineNumber) {
    int lineIndex = SendMessage(hEdit, EM_LINEINDEX, lineNumber - 1, 0);
    int nextLineIndex = SendMessage(hEdit, EM_LINEINDEX, lineNumber, 0);
    if (nextLineIndex == -1) {
        nextLineIndex = GetWindowTextLength(hEdit);
    }
    SendMessage(hEdit, EM_SETSEL, lineIndex, lineIndex);
    SendMessage(hEdit, EM_SCROLLCARET, 0, 0);
}

void Compilacao(HWND hwnd){
    // Limpar mensagens de erro anteriores
    SetWindowTextA(hErros, "");

    // Verificar se há conteúdo para compilar
    int length = GetWindowTextLengthA(hCodigo);
    if (length == 0) {
        MessageBoxW(hwnd, L"Nenhum código para compilar.", L"Erro", MB_OK | MB_ICONERROR);
        return;
    }

    // Verificar se há um arquivo aberto
    wchar_t fileName[MAX_PATH];
    GetWindowTextW(hArquivo, fileName, MAX_PATH);

    // Se não houver arquivo aberto, usar "Salvar Como"
    if (wcslen(fileName) == 0) {
        SalvarArquivo(hwnd);
        
        // Recuperar o nome do arquivo após salvar
        GetWindowTextW(hArquivo, fileName, MAX_PATH);
        
        // Se o usuário cancelou o salvamento, sair
        if (wcslen(fileName) == 0) {
            return;
        }
    } else {
        // Se já houver um arquivo aberto, salvar no arquivo original
        SalvarArquivoOriginal(hwnd);
    }

    // Abrir o arquivo para compilação
    FILE *fp = _wfopen(fileName, L"r");
    if (fp == NULL) {
        MessageBoxW(hwnd, L"Erro ao abrir o arquivo para compilação.", L"Erro", MB_OK | MB_ICONERROR);
        return;
    }

    // Preparar variáveis para análise sintática
    char erro[1024] = {0};
    
    int linha_erro = compilar(fp, erro);;

    // Fechar arquivo
    fclose(fp);

    limpa_memoria();

    // Verificar se há erros
    if (strlen(erro) > 0) {
        // Exibir erros no controle de erros
        SetWindowTextA(hErros, erro);
        if (linha_erro > 0) {
            SetCursorToLine(hCodigo, linha_erro);
        }
    } else {
        // Se não houver erros, mostrar mensagem de sucesso
        SetWindowTextA(hErros, "Compilação concluída com sucesso!");
    }
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            AddMenus(hwnd);
            AddControls(hwnd);
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case ID_BTN_COMPILAR:
                    Compilacao(hwnd);
                    break;
                case ID_FILE_OPEN:
                    AbrirArquivo(hwnd);
                    break;
                case ID_FILE_SAVE:
                    SalvarArquivo(hwnd);
                    break;
                case ID_FILE_SAVE2:
                    SalvarArquivoOriginal(hwnd);
                    break;
                case ID_FILE_CLOSE:
                    CloseFile(hwnd);
                    break;
                case ID_FILE_EXIT:
                    DestroyWindow(hwnd);
                    break;
                case ID_HELP_ABOUT:
                    MessageBoxW(hwnd, L"Compilador v1.0\nDesenvolvido como exemplo", L"Sobre", MB_OK | MB_ICONINFORMATION);
                    break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void AddControls(HWND hwnd) {
    // Label Arquivo
    CreateWindowW(L"Static", L"Arquivo:",
                 WS_VISIBLE | WS_CHILD,
                 10, 10, 50, 20,
                 hwnd, NULL, NULL, NULL);

    // Campo de arquivo
    hArquivo = CreateWindowW(L"Edit", L"",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                            70, 10, 600, 20,
                            hwnd, (HMENU)ID_EDIT_ARQUIVO, NULL, NULL);

    // Área de código
    hCodigo = CreateWindowW(L"Edit", L"",
                           WS_VISIBLE | WS_CHILD | WS_BORDER | 
                           WS_VSCROLL | WS_HSCROLL | 
                           ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                           10, 40, 760, 400,
                           hwnd, (HMENU)ID_EDIT_CODIGO, NULL, NULL);

    // Status da linha e coluna
    hStatus = CreateWindowW(L"Static", L"Ln 1, Col 1",
                           WS_VISIBLE | WS_CHILD,
                           10, 450, 100, 20,
                           hwnd, (HMENU)ID_STATUSBAR, NULL, NULL);

    // Label Erros
    CreateWindowW(L"Static", L"Erros:",
                 WS_VISIBLE | WS_CHILD,
                 10, 480, 50, 20,
                 hwnd, NULL, NULL, NULL);

    // Área de erros
    hErros = CreateWindowW(L"Edit", L"",
                          WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_AUTOHSCROLL,
                          10, 500, 650, 40,
                          hwnd, (HMENU)ID_EDIT_ERROS, NULL, NULL);

    // Botão Compilar
    hCompile = CreateWindowW(L"Button", L"Compilar",
                            WS_VISIBLE | WS_CHILD,
                            670, 500, 100, 40,
                            hwnd, (HMENU)ID_BTN_COMPILAR, NULL, NULL);
}