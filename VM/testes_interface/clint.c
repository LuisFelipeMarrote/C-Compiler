#include <windows.h>
#include <commctrl.h>

// Window procedure declaration
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Global variables
HINSTANCE g_hInst;
HWND g_hListView, g_hMemoryListView, g_hOutputEdit;
HWND g_hNormalRadio, g_hStepByStepRadio;
HWND g_hExecuteButton, g_hStopButton;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, int nCmdShow) {
    
    g_hInst = hInstance;
    
    // Register window class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
        {
            // Create main ListView (Code table)
            g_hListView = CreateWindowEx(
                0,
                WC_LISTVIEW,
                "",
                WS_CHILD | WS_VISIBLE | LVS_REPORT,
                10, 10, 500, 300,
                hwnd, NULL, g_hInst, NULL
            );
            
            // Add columns to main ListView
            LVCOLUMN lvc = {0};
            lvc.mask = LVCF_TEXT | LVCF_WIDTH;
            
            lvc.pszText = "Linha";
            lvc.cx = 50;
            ListView_InsertColumn(g_hListView, 0, &lvc);
            
            lvc.pszText = "Instrução";
            lvc.cx = 80;
            ListView_InsertColumn(g_hListView, 1, &lvc);
            
            lvc.pszText = "Atributo 1";
            lvc.cx = 80;
            ListView_InsertColumn(g_hListView, 2, &lvc);
            
            lvc.pszText = "Atributo 2";
            lvc.cx = 80;
            ListView_InsertColumn(g_hListView, 3, &lvc);
            
            lvc.pszText = "Comentário";
            lvc.cx = 200;
            ListView_InsertColumn(g_hListView, 4, &lvc);
            
            // Create Memory ListView
            g_hMemoryListView = CreateWindowEx(
                0,
                WC_LISTVIEW,
                "",
                WS_CHILD | WS_VISIBLE | LVS_REPORT,
                520, 10, 250, 300,
                hwnd, NULL, g_hInst, NULL
            );
            
            // Add columns to Memory ListView
            lvc.pszText = "Endereço";
            lvc.cx = 100;
            ListView_InsertColumn(g_hMemoryListView, 0, &lvc);
            
            lvc.pszText = "Valor";
            lvc.cx = 100;
            ListView_InsertColumn(g_hMemoryListView, 1, &lvc);
            
            // Create Output Edit box
            g_hOutputEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                "EDIT",
                "",
                WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL,
                10, 320, 300, 100,
                hwnd, NULL, g_hInst, NULL
            );
            
            // Create Radio buttons
            g_hNormalRadio = CreateWindowEx(
                0,
                "BUTTON",
                "Normal",
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                320, 320, 100, 30,
                hwnd, NULL, g_hInst, NULL
            );
            
            g_hStepByStepRadio = CreateWindowEx(
                0,
                "BUTTON",
                "Passo a Passo",
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                320, 350, 100, 30,
                hwnd, NULL, g_hInst, NULL
            );
            
            // Create Execute and Stop buttons
            g_hExecuteButton = CreateWindowEx(
                0,
                "BUTTON",
                "Executar",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                440, 320, 100, 30,
                hwnd, NULL, g_hInst, NULL
            );
            
            g_hStopButton = CreateWindowEx(
                0,
                "BUTTON",
                "Parar",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                440, 360, 100, 30,
                hwnd, NULL, g_hInst, NULL
            );
            
            // Set default radio button
            SendMessage(g_hStepByStepRadio, BM_SETCHECK, BST_CHECKED, 0);
            
            return 0;
        }
        
        case WM_COMMAND:
            // Handle button clicks and other commands here
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
