#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <richedit.h>
#include "novaVMD.h"

#define ID_LISTVIEW_CODE 1001
#define ID_LISTVIEW_MEMORY 1002
#define ID_EDIT_OUTPUT 1003
#define ID_RADIO_NORMAL 1004
#define ID_RADIO_STEP 1005
#define ID_BTN_EXECUTE 1006
#define ID_BTN_STOP 1007
#define ID_FILE_OPEN 1008
#define ID_FILE_EXIT 1009
#define ID_HELP_ABOUT 1010

BOOL isStepMode = TRUE;  // TRUE for step-by-step, FALSE for normal
BOOL isExecuting = FALSE; // Control execution state

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void AddControls(HWND hwnd);
void AddMenus(HWND hwnd);
void InitListViewColumns(HWND hList, BOOL isCode);
void AbrirArquivo(HWND hwnd);
void lerInstrucoes(FILE *file);
void AddIntructionItem(const char* linha, const char* rotulo, const char* instrucao, 
                const char* attr1, const char* attr2, const char* comentario);
void LimparRecursos();
void execucao_normal(HWND hwnd);
void ExecuteStep(HWND hwnd);
void UpdateMemoryView();
void UpdateOutputText(const char* text);
void LimparRecursosIntermediarios();


HWND hListCode, hListMemory, hOutput, hRadioNormal, hRadioStep, hExecute, hStop;
HMENU hMenuBar = NULL;
HMENU hFileMenu = NULL;
HMENU hHelpMenu = NULL;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"MaquinaVirtual";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc))
        return -1;

    CreateWindowW(L"MaquinaVirtual", L"Máquina Virtual",
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
    hMenuBar = CreateMenu();
    hFileMenu = CreateMenu();
    hHelpMenu = CreateMenu();

    AppendMenuW(hFileMenu, MF_STRING, ID_FILE_OPEN, L"Abrir");
    AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hFileMenu, MF_STRING, ID_FILE_EXIT, L"Sair");

    AppendMenuW(hHelpMenu, MF_STRING, ID_HELP_ABOUT, L"Sobre");

    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"Arquivo");
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hHelpMenu, L"Sobre");

    SetMenu(hwnd, hMenuBar);
}

void InitListViewColumns(HWND hList, BOOL isCode) {
    LVCOLUMNW lvc = {0};
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    if (isCode) {
        // Columns for machine code
        lvc.cx = 50;
        lvc.pszText = L"Linha";
        ListView_InsertColumn(hList, 0, &lvc);

        lvc.cx = 50;
        lvc.pszText = L"Rótulo";
        ListView_InsertColumn(hList, 1, &lvc);

        lvc.cx = 80;
        lvc.pszText = L"Instrução";
        ListView_InsertColumn(hList, 2, &lvc);

        lvc.cx = 80;
        lvc.pszText = L"Atributo 1";
        ListView_InsertColumn(hList, 3, &lvc);

        lvc.cx = 80;
        lvc.pszText = L"Atributo 2";
        ListView_InsertColumn(hList, 4, &lvc);
        
        /*lvc.cx = 200;
        lvc.pszText = L"Comentário";
        ListView_InsertColumn(hList, 5, &lvc);*/
    } else {
        // Columns for memory
        lvc.cx = 100;
        lvc.pszText = L"Endereço";
        ListView_InsertColumn(hList, 0, &lvc);

        lvc.cx = 100;
        lvc.pszText = L"Valor";
        ListView_InsertColumn(hList, 1, &lvc);
    }
}

void LimparListViews() {
    // Limpa a ListView de código
    ListView_DeleteAllItems(hListCode);
    
    // Limpa a ListView de memória
    ListView_DeleteAllItems(hListMemory);
    
    // Limpa o campo de output
    SetWindowTextW(hOutput, L"");
}

void AddIntructionItem(const char* linha, const char* rotulo, const char* instrucao, 
                const char* attr1, const char* attr2, const char* comentario) {
    LVITEM lvi = {0};
    lvi.mask = LVIF_TEXT;
    
    int itemCount = ListView_GetItemCount(hListCode);
    
    lvi.iItem = itemCount;
    lvi.iSubItem = 0;
    lvi.pszText = (char*)linha;
    ListView_InsertItem(hListCode, &lvi);
    
    ListView_SetItemText(hListCode, itemCount, 1, (char*)rotulo);
    ListView_SetItemText(hListCode, itemCount, 2, (char*)instrucao);
    ListView_SetItemText(hListCode, itemCount, 3, (char*)attr1);
    ListView_SetItemText(hListCode, itemCount, 4, (char*)attr2);
    //ListView_SetItemText(hListCode, itemCount, 5, (char*)comentario);
}

void lerInstrucoes(FILE *file) {
    int count = -1;
    while (strcmp(lista[count].instrucao, "HLT     ") != 0){
        (count)++;

        // Analisa e preenche os campos da estrutura
        fgets(lista[count].rotulo, sizeof(lista[count].rotulo), file);
        fgets(lista[count].instrucao, sizeof(lista[count].instrucao), file);
        fgets(lista[count].atr1, sizeof(lista[count].atr1), file);
        fgets(lista[count].atr2, sizeof(lista[count].atr2), file);
 
        char buffer[12];
        snprintf(buffer, sizeof(buffer), "%d", count);
        AddIntructionItem(buffer, lista[count].rotulo, lista[count].instrucao, lista[count].atr1, lista[count].atr2, " ");
        
        // Imprime as informações lidas (opcional)
        /*printf("%4s %8s %4s %4s\n",
               lista[count].rotulo,
               lista[count].instrucao,
               lista[count].atr1,
               lista[count].atr2);*/
    }
}

void AbrirArquivo(HWND hwnd) {
    OPENFILENAMEW ofn;
    wchar_t szFile[260] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile)/sizeof(wchar_t);
    ofn.lpstrFilter = L"Arquivos obj (*.obj)\0*.obj\0Todos os Arquivos (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        LimparListViews();
        countres = 0;
        //zerar a pilha

        FILE *file = _wfopen(ofn.lpstrFile, L"r");
        if (file) {
            lerInstrucoes(file);
            fclose(file);
        }else {
            MessageBoxW(hwnd, L"Erro ao abrir o arquivo.", L"Erro", MB_OK | MB_ICONERROR);
        }
    }
}

void LimparRecursos() {
    // Destroi os menus
    if (hFileMenu) DestroyMenu(hFileMenu);
    if (hHelpMenu) DestroyMenu(hHelpMenu);
    if (hMenuBar) DestroyMenu(hMenuBar);

    // Libera qualquer memória global que tenha sido alocada
    LimparRecursosIntermediarios();
    // Adicione aqui outras liberações necessárias para suas estruturas globais
}

void LimparRecursosIntermediarios(){
    countres = 0;
    i = 0;
    liberarPilha();
}

void execucao_normal(HWND hwnd) {
    if (strcmp(lista[0].instrucao, "START   ") != 0) {
        MessageBoxW(hwnd, L"Erro ao abrir o arquivo.", L"Erro", MB_OK | MB_ICONERROR);
        isExecuting = FALSE;
        EnableWindow(hExecute, TRUE);
        return;
    }
    while (isExecuting && strcmp(lista[countres].instrucao, "HLT     ") != 0) {
        //resolveInst(&countres);
        countres++; //teste apenas
        UpdateMemoryView();
        
        // Optional: Add a small delay to show progress
        Sleep(100);
        
        // Update interface
        ListView_SetItemState(hListCode, countres, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
        ListView_EnsureVisible(hListCode, countres, FALSE);
    }
    if (isExecuting) {
        MessageBoxW(hwnd, L"Programa finalizado", L"Informação", MB_OK | MB_ICONINFORMATION);
    }

    isExecuting = FALSE;
    EnableWindow(hExecute, TRUE);
    LimparRecursosIntermediarios();
}

void ExecuteStep(HWND hwnd) {
    if (!isExecuting) return;
    
    // Check if it's the first instruction
    if (countres == 0 && strcmp(lista[0].instrucao, "START   ") != 0) {
        MessageBoxW(hwnd, L"Erro: Programa deve começar com START", L"Erro", MB_OK | MB_ICONERROR);
        isExecuting = FALSE;
        EnableWindow(hExecute, TRUE);
        return;
    }

    // Check if we've reached HLT
    if (strcmp(lista[countres].instrucao, "HLT     ") == 0) {
        MessageBoxW(hwnd, L"Programa finalizado", L"Informação", MB_OK | MB_ICONINFORMATION);
        isExecuting = FALSE;
        EnableWindow(hExecute, TRUE);
        return;
    }

    // Execute single instruction
    //resolveInst(&countres);
    countres++ ;// teste apenas

    // Update the interface
    UpdateMemoryView();
    
    // Highlight current instruction in ListView
    ListView_SetItemState(hListCode, countres, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    ListView_EnsureVisible(hListCode, countres, FALSE);
}

void UpdateOutputText(const char* text) {
    // Get current text length
    int length = GetWindowTextLengthW(hOutput);
    
    // Append new text
    SendMessageW(hOutput, EM_SETSEL, length, length);
    SendMessageA(hOutput, EM_REPLACESEL, FALSE, (LPARAM)text);
}

void UpdateMemoryView() {
    ListView_DeleteAllItems(hListMemory);
    
    if(p == NULL){
        return;
    }
    // Add current stack contents to memory view
    LVITEM lvi = {0};
    lvi.mask = LVIF_TEXT;
    
    for (int i = 0; i <= (p->s); i++) {
        char endereco[16], valor[16];
        snprintf(endereco, sizeof(endereco), "%d", i);
        snprintf(valor, sizeof(valor), "%d", p[i]);
        
        lvi.iItem = i;
        lvi.iSubItem = 0;
        lvi.pszText = endereco;
        ListView_InsertItem(hListMemory, &lvi);
        
        ListView_SetItemText(hListMemory, i, 1, valor);
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
                case ID_FILE_OPEN:
                    AbrirArquivo(hwnd);
                    break;
                case ID_FILE_EXIT:
                    DestroyWindow(hwnd);
                    break;
                case ID_HELP_ABOUT:
                    MessageBoxW(hwnd, L"Máquina Virtual v1.0", L"Sobre", MB_OK | MB_ICONINFORMATION);
                    break;
                case ID_RADIO_NORMAL:
                    isStepMode = FALSE;
                    break;
                case ID_RADIO_STEP:
                    isStepMode = TRUE;
                    break;
                case ID_BTN_EXECUTE:
                    if (!isExecuting) {
                        isExecuting = TRUE;
                        EnableWindow(hExecute, FALSE);  // Disable execute button
                        if (isStepMode) {
                            ExecuteStep(hwnd);
                            //MessageBoxW(hwnd, L"ainda nao implementei execucao passo a passo", L"Erro", MB_OK | MB_ICONERROR);  
                        } else {
                            execucao_normal(hwnd);
                        }
                    }
                    break;
                case ID_BTN_STOP:
                    isExecuting = FALSE;
                    EnableWindow(hExecute, TRUE);  // Re-enable execute button
                    LimparRecursosIntermediarios();
                    break;
            }
            break;
        case WM_DESTROY:
            LimparRecursos();
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void AddControls(HWND hwnd) {
    // Código de Máquina Group
    CreateWindowW(L"Button", L"Código de Máquina",
                 WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                 10, 10, 500, 300,
                 hwnd, NULL, NULL, NULL);

    // Machine Code ListView
    hListCode = CreateWindowW(WC_LISTVIEWW, L"",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
                            20, 30, 480, 270,
                            hwnd, (HMENU)ID_LISTVIEW_CODE, NULL, NULL);
    InitListViewColumns(hListCode, TRUE);

    // Memória (Pilha) Group
    CreateWindowW(L"Button", L"Memória (Pilha)",
                 WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                 520, 10, 250, 300,
                 hwnd, NULL, NULL, NULL);

    // Memory ListView
    hListMemory = CreateWindowW(WC_LISTVIEWW, L"",
                              WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
                              530, 30, 230, 270,
                              hwnd, (HMENU)ID_LISTVIEW_MEMORY, NULL, NULL);
    InitListViewColumns(hListMemory, FALSE);

    // Saída de Dados Group
    CreateWindowW(L"Button", L"Saída de Dados",
                 WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                 10, 320, 300, 150,
                 hwnd, NULL, NULL, NULL);

    // Output Edit
    hOutput = CreateWindowW(L"Edit", L"",
                          WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
                          20, 340, 280, 120,
                          hwnd, (HMENU)ID_EDIT_OUTPUT, NULL, NULL);

    // Modo de Execução Group
    CreateWindowW(L"Button", L"Modo de Execução",
                 WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                 320, 320, 200, 100,
                 hwnd, NULL, NULL, NULL);

    // Radio buttons
    hRadioNormal = CreateWindowW(L"Button", L"Normal",
                               WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                               330, 340, 180, 30,
                               hwnd, (HMENU)ID_RADIO_NORMAL, NULL, NULL);

    hRadioStep = CreateWindowW(L"Button", L"Passo a Passo",
                             WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                             330, 370, 180, 30,
                             hwnd, (HMENU)ID_RADIO_STEP, NULL, NULL);

    // Set default radio button
    SendMessage(hRadioStep, BM_SETCHECK, BST_CHECKED, 0);

    // Control buttons
    hExecute = CreateWindowW(L"Button", L"Executar",
                           WS_VISIBLE | WS_CHILD,
                           530, 340, 100, 30,
                           hwnd, (HMENU)ID_BTN_EXECUTE, NULL, NULL);

    hStop = CreateWindowW(L"Button", L"Parar",
                         WS_VISIBLE | WS_CHILD,
                         640, 340, 100, 30,
                         hwnd, (HMENU)ID_BTN_STOP, NULL, NULL);
}