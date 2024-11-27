#include <windows.h>
#include <commctrl.h>
#include <locale.h>
#include <commdlg.h> 
#include <stdio.h>

// Window procedure declaration
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Global variables
HINSTANCE g_hInst;
HWND g_hListView, g_hMemoryListView, g_hOutputEdit;
HWND g_hNormalRadio, g_hStepByStepRadio,hGrpButtons;
HWND g_hExecuteButton, g_hStopButton;
HMENU g_hMainMenu;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, int nCmdShow) {
    
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Configura o idioma para português (Brasil)
    
    g_hInst = hInstance;
    
    // Register window class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = "VirtualMachineClass";
    RegisterClassEx(&wc);
    
    // Initialize Common Controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);
    
    // Create main window
    HWND hwnd = CreateWindowEx(
        0,
        "VirtualMachineClass",
        "Máquina Virtual",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );
    
    if (!hwnd) return FALSE;
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}

void AddMemoryItem(const char* linha, const char* instrucao, 
                const char* attr1, const char* attr2, const char* comentario) {
    LVITEM lvi = {0};
    lvi.mask = LVIF_TEXT;
    
    int itemCount = ListView_GetItemCount(g_hListView);
    
    lvi.iItem = itemCount;
    lvi.iSubItem = 0;
    lvi.pszText = (char*)linha;
    ListView_InsertItem(g_hListView, &lvi);
    
    ListView_SetItemText(g_hListView, itemCount, 1, (char*)instrucao);
    ListView_SetItemText(g_hListView, itemCount, 2, (char*)attr1);
    ListView_SetItemText(g_hListView, itemCount, 3, (char*)attr2);
    ListView_SetItemText(g_hListView, itemCount, 4, (char*)comentario);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
        {
            //define os tamanhos em relação à janela
            RECT parentRect;
            GetClientRect(hwnd, &parentRect);
            int parent_height = (parentRect.bottom - parentRect.top);
            int parent_Width = (parentRect.right - parentRect.left);

            //janela de codigo:
            int hlistViewWidth = parent_Width * 0.76;
            int hlistViewHeight = parent_height * 0.7;
            int hlistviewX = 10;
            int hlistviewY = 10;

            //janela de memoria:
            int memoryListWidth = parent_Width * 0.2;
            int memorylistX = hlistviewX + hlistViewWidth + 14;
            int memorylistY = hlistviewY;

            //saída de dados:
            int outputWidth = parent_Width * 0.57;
            int outputHeight = parent_height * 0.22;
            int outputX = hlistviewX;
            int outputY = hlistviewY + hlistViewHeight + 5;

            //botoes radio: 
            int caixaWidth = 200;
            int caixaHeight = 90;
            int caixaX = outputX + outputWidth + 10;
            int caixaY = outputY + ((outputHeight/2) - (caixaHeight/2));

            int normalWidth = caixaWidth - 20;
            int normalHeight = 30;
            int normalX = caixaX + 10;
            int normalY = caixaY + 20;
            
            int passoWidth = normalWidth;
            int passoHeight = 30;
            int passoX = caixaX + 10;
            int passoY = normalY + normalHeight;
            
            //botoes execução:
            int executarWidth = 100;
            int executarHeight = 30;
            int executarX = caixaX + caixaWidth + 10;
            int executarY = caixaY + 15;
            
            int pararWidth = 100;
            int pararHeight = 30;
            int pararX = executarX;
            int pararY = executarY + executarHeight + 10;

            // Menu
            g_hMainMenu = CreateMenu();
            HMENU hFileMenu = CreatePopupMenu();
            AppendMenu(hFileMenu, MF_STRING, 1000, "Abrir");
            AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
            AppendMenu(hFileMenu, MF_STRING, 1001, "Sair");

            AppendMenu(g_hMainMenu, MF_POPUP, (UINT_PTR)hFileMenu, "Arquivo");
            
            SetMenu(hwnd, g_hMainMenu);

            // Create main ListView (Code table)
            g_hListView = CreateWindowEx(
                0,
                WC_LISTVIEW,
                "",
                WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_NOSORTHEADER |
                WS_BORDER | WS_VSCROLL | WS_HSCROLL,  // Adiciona bordas e barras de rolagem
                hlistviewX, hlistviewY,     // posição x, y
                hlistViewWidth, hlistViewHeight,   // largura, altura fixa
                hwnd, NULL, g_hInst, NULL
            );
            
            RECT rect;
            GetClientRect(g_hListView, &rect);  // Pega o tamanho total do ListView
            int totalWidth = rect.right - rect.left;  // Calcula a largura total

            // Add columns to main ListView
            LVCOLUMN lvc = {0};
            lvc.mask = LVCF_TEXT | LVCF_WIDTH;
            
            lvc.pszText = "Linha";
            lvc.cx = totalWidth * 0.10;
            ListView_InsertColumn(g_hListView, 0, &lvc);
            
            lvc.pszText = "Instrução";
            lvc.cx = totalWidth * 0.20;
            ListView_InsertColumn(g_hListView, 1, &lvc);
            
            lvc.pszText = "Atributo 1";
            lvc.cx = totalWidth * 0.20;
            ListView_InsertColumn(g_hListView, 2, &lvc);
            
            lvc.pszText = "Atributo 2";
            lvc.cx = totalWidth * 0.20;
            ListView_InsertColumn(g_hListView, 3, &lvc);
            
            lvc.pszText = "Comentário";
            lvc.cx = totalWidth * 0.31;
            ListView_InsertColumn(g_hListView, 4, &lvc);
            
            // Create Memory ListView
            g_hMemoryListView = CreateWindowEx(
                0,
                WC_LISTVIEW,
                "",
                WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_NOSORTHEADER |
                WS_BORDER | WS_VSCROLL | WS_HSCROLL,
                memorylistX, memorylistY, memoryListWidth, hlistViewHeight,
                hwnd, NULL, g_hInst, NULL
            );
            ListView_SetExtendedListViewStyle(g_hMemoryListView, LVS_EX_GRIDLINES);
            
            GetClientRect(g_hMemoryListView, &rect);  // Pega o tamanho total do ListView
            totalWidth = rect.right - rect.left;  // Calcula a largura total

            // Add columns to Memory ListView
            lvc.pszText = "Endereço";
            lvc.cx = totalWidth * 0.50;
            ListView_InsertColumn(g_hMemoryListView, 0, &lvc);
            
            lvc.pszText = "Valor";
            lvc.cx = totalWidth * 0.51;
            ListView_InsertColumn(g_hMemoryListView, 1, &lvc);
            
            // Create Output Edit box
            g_hOutputEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                "EDIT",
                "Saída",
                WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL,
                outputX, outputY, outputWidth, outputHeight,
                hwnd, NULL, g_hInst, NULL
            );
            
            // Create Radio buttons
            
            hGrpButtons=CreateWindowEx(
                WS_EX_WINDOWEDGE,
                "BUTTON",
                "Select Process Mode:", 
                WS_VISIBLE | WS_CHILD|BS_GROUPBOX,
                caixaX,caixaY,
                caixaWidth,caixaHeight, 
                hwnd, /*(HMENU)IDC_GRPBUTTONS*/ NULL, g_hInst, NULL
            );

            g_hNormalRadio = CreateWindowEx(
                0,
                "BUTTON",
                "Normal",
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
                normalX, normalY, normalWidth, normalHeight,
                hwnd, NULL, g_hInst, NULL
            );
            
            g_hStepByStepRadio = CreateWindowEx(
                0,
                "BUTTON",
                "Passo a Passo",
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                passoX, passoY, passoWidth, passoHeight,
                hwnd, NULL, g_hInst, NULL
            );
            
            // Create Execute and Stop buttons
            g_hExecuteButton = CreateWindowEx(
                0,
                "BUTTON",
                "Executar",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                executarX, executarY, executarWidth, executarHeight,
                hwnd, NULL, g_hInst, NULL
            );
            
            g_hStopButton = CreateWindowEx(
                0,
                "BUTTON",
                "Parar",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                pararX, pararY, pararWidth, pararHeight,
                hwnd, NULL, g_hInst, NULL
            );
            
            char str1[12] = " ", str2[12] = " ";
            sprintf(str1, "%d", parent_height);
            sprintf(str2, "%d", parent_Width);
            AddMemoryItem("1", "altura", str1, "largura", str2);
            AddMemoryItem("2", "teste", "sla", "sl2", "str2");


            // Set default radio button
            SendMessage(g_hStepByStepRadio, BM_SETCHECK, BST_CHECKED, 0);
            
            return 0;
        }
        
        /*case WM_SIZE:
        {
            // Get new window dimensions
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);

            // Reposition and resize controls proportionally
            RECT parentRect;
            GetClientRect(hwnd, &parentRect);

            // Code ListView
            int hlistViewWidth = width * 0.76;
            int hlistViewHeight = height * 0.7;
            MoveWindow(g_hListView, 10, 10, hlistViewWidth, hlistViewHeight, TRUE);

            // Memory ListView
            int memoryListWidth = width * 0.2;
            int memorylistX = 10 + hlistViewWidth + 14;
            MoveWindow(g_hMemoryListView, memorylistX, 10, memoryListWidth, hlistViewHeight, TRUE);

            // Output Edit box
            int outputWidth = width * 0.6;
            int outputHeight = height * 0.22;
            MoveWindow(g_hOutputEdit, 10, 10 + hlistViewHeight + 5, outputWidth, outputHeight, TRUE);

            // Adjust radio buttons and other controls as needed
            // You'll want to calculate new positions based on width and height
            MoveWindow(g_hNormalRadio, 320, height * 0.6, 100, 30, TRUE);
            MoveWindow(g_hStepByStepRadio, 320, height * 0.6 + 30, 100, 30, TRUE);
            MoveWindow(g_hExecuteButton, 440, height * 0.6, 100, 30, TRUE);
            MoveWindow(g_hStopButton, 440, height * 0.6 + 40, 100, 30, TRUE);

            return 0;
        }*/

        case WM_COMMAND:
            // Handle button clicks and other commands here
            //rever isso tudo! (placeholder)
            switch (LOWORD(wParam))
                {
                    case 1000: // ID para "Abrir"
                    {
                        OPENFILENAME ofn;
                        char szFile[260] = {0};

                        ZeroMemory(&ofn, sizeof(ofn));
                        ofn.lStructSize = sizeof(ofn);
                        ofn.hwndOwner = hwnd;
                        ofn.lpstrFile = szFile;
                        ofn.nMaxFile = sizeof(szFile);
                        ofn.lpstrFilter = "Arquivos de Texto (*.txt)\0*.txt\0Todos os Arquivos (*.*)\0*.*\0";
                        ofn.nFilterIndex = 1;
                        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                        if (GetOpenFileName(&ofn)==TRUE) 
                        {
                            // Aqui você pode adicionar código para processar o arquivo aberto
                            // Por exemplo, ler o conteúdo do arquivo
                            FILE *file = fopen(ofn.lpstrFile, "r");
                            if (file) 
                            {
                                // Limpa o conteúdo anterior do EditControl
                                SetWindowText(g_hOutputEdit, "");

                                /*char buffer[256];
                                while (fgets(buffer, sizeof(buffer), file)) 
                                {
                                    // Adiciona cada linha ao EditControl
                                    SendMessage(g_hOutputEdit, EM_REPLACESEL, 0, (LPARAM)buffer);
                                }
                                fclose(file);*/
                            }
                        }
                        break;
                    }
                    case 1001: // ID para "Sair"
                        PostMessage(hwnd, WM_CLOSE, 0, 0);
                        break;
                }
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
