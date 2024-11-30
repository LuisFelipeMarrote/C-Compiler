#include <windows.h>
#include <commctrl.h>
#include <locale.h>
#include <commdlg.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INST 256
#define PILHA_TAM_INICIAL    64    
#define PILHA_TAM_MAXIMO    1024 
#define MAX_ROTULOS    256 


#define ID_NORMAL_RADIO    201
#define ID_STEP_RADIO      202
#define ID_EXECUTE_BUTTON  203
#define ID_STOP_BUTTON     204
#define IDC_INPUT_FIELD 1001
#define IDOK 1
#define IDCANCEL 2

// Window procedure declaration
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Global variables
HINSTANCE g_hInst;
HWND g_hListView, g_hMemoryListView, g_hOutputEdit;
HWND g_hNormalRadio, g_hStepByStepRadio,hGrpButtons;
HWND hwnd;
HWND g_hExecuteButton, g_hStopButton;
HMENU g_hMainMenu;
int g_executionMode  = 1;

//pilha de dados
typedef struct {
    int* M; //Memória da pilha
    int s; //Topo da pilha
    int capacidade; // capacidade atual da pilha
    int max_capacidade; // capacidade máxima para evitar ovelflow
} Pilha;

struct Inst {
    char rotulo[5];
    char instrucao[9];
    char atr1[5];
    char atr2[6];
};

Pilha *p;
int i = 0;
struct Inst lista[MAX_INST];
int countres = -1;
int *rotulos;

Pilha* inicializarPilha(int capacidadeInicial);
int pilhaVazia(); 
int pilhaCheia();
int empilhar(int atributo);
int desempilhar(int *valor);
int topo();
int redimensionarPilha();
void liberarPilha();
int analisaInst(struct Inst *lista);
void lerInstrucoes(FILE *file);
void resolveInst(int* count);
void MVD();

void AddIntructionItem(const char* linha, const char* rotulo, const char* instrucao, 
                const char* attr1, const char* attr2, const char* comentario) {
    LVITEM lvi = {0};
    lvi.mask = LVIF_TEXT;
    
    int itemCount = ListView_GetItemCount(g_hListView);
    
    lvi.iItem = itemCount;
    lvi.iSubItem = 0;
    lvi.pszText = (char*)linha;
    ListView_InsertItem(g_hListView, &lvi);
    
    ListView_SetItemText(g_hListView, itemCount, 1, (char*)rotulo);
    ListView_SetItemText(g_hListView, itemCount, 2, (char*)instrucao);
    ListView_SetItemText(g_hListView, itemCount, 3, (char*)attr1);
    ListView_SetItemText(g_hListView, itemCount, 4, (char*)attr2);
    ListView_SetItemText(g_hListView, itemCount, 5, (char*)comentario);
}

void AtualizarListViewMemoria() {
    // Limpa a ListView de memória primeiro
    ListView_DeleteAllItems(g_hMemoryListView);

    // Verifica se a pilha está vazia
    if (p == NULL || pilhaVazia()) {
        return;
    }

    // Percorre todos os elementos da pilha
    for (int i = 0; i <= p->s; i++) {
        char enderecoStr[20];
        char valorStr[20];

        // Formata o endereço e o valor
        sprintf(enderecoStr, "Pilha[%d]", i);
        sprintf(valorStr, "%d", p->M[i]);

        // Adiciona item à ListView
        LVITEM lvi = {0};
        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;
        lvi.iSubItem = 0;
        lvi.pszText = enderecoStr;
        ListView_InsertItem(g_hMemoryListView, &lvi);

        // Define o valor na segunda coluna
        ListView_SetItemText(g_hMemoryListView, i, 1, valorStr);
    }
}

// Função para executar o programa
void ExecutarPrograma() {
    // Verificar se há instruções carregadas
    if (countres == -1) {
        MessageBox(NULL, "Nenhum programa carregado.", "Erro", MB_OK | MB_ICONERROR);
        return;
    }

    // Reinicializar a pilha se necessário
    if (p == NULL) {
        p = inicializarPilha(PILHA_TAM_INICIAL);
    }

    // Executar o programa
    if (g_executionMode == 1) { // Modo Passo a Passo
        // No modo passo a passo, você pode querer adicionar uma função que
        // executa uma instrução por vez quando o usuário clica em "Próximo"
        // ou criar um mecanismo de pausa entre instruções
        InvalidateRect(g_hListView, NULL, TRUE); // Redesenha o ListView
        countres++;
        resolveInst(&countres);
        AtualizarListViewMemoria();
        
    } else { // Modo Normal
        countres = 0;
        // Limpar a saída
        SetWindowText(g_hOutputEdit, "");
        // Executar todas as instruções de uma vez
        MVD(NULL);
        AtualizarListViewMemoria();
    }
}

// Função para parar a execução
void PararExecucao() {
    OutputDebugString("Programa entrou na parada de execução\n");
    // Limpar a pilha
    if (p != NULL) {
        liberarPilha();
        p = NULL;
    }
    OutputDebugString("liberou a pilha\n");

    // Limpar contadores e estados
    countres = 0;

    // Limpar a ListView de memória
    ListView_DeleteAllItems(g_hMemoryListView);

    InvalidateRect(g_hListView, NULL, TRUE); // Redesenha o ListView

    // Limpar a saída
    SetWindowText(g_hOutputEdit, "Execução interrompida.");
}

LRESULT CALLBACK CustomDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static char* entradaStr;

    switch (message) {
        case WM_INITDIALOG:
            //entradaStr = (char*)lParam; // Armazena o buffer
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case IDOK: {
                char buffer[12];
                //sprintf(buffer, "%d", (sizeof(entradaStr) / sizeof(entradaStr[0])));
                // Obtém o texto digitado no campo
                //itoa((sizeof(entradaStr) / sizeof(entradaStr[0])), buffer,10);
                //memcpy(entradaStr,buffer,(sizeof(entradaStr) / sizeof(entradaStr[0])));
                GetDlgItemText(hDlg, IDC_INPUT_FIELD, buffer, sizeof(buffer));
                if (!entradaStr){
                    //SetWindowText(g_hOutputEdit, "str");
                    //char* str = buffer;
                    //SetWindowText(g_hOutputEdit, buffer);
                    int entrada = atoi(buffer);
                    empilhar(entrada);
                    
                }else{
                    //char str[30] = "--------------------";
                    //char* ajuda = "///////";
                    //strcat(str, ajuda);
                    //SetWindowText(g_hOutputEdit, "buffer");
                }
                DestroyWindow(hDlg); // Fecha o diálogo
                PostQuitMessage(0);  // Finaliza o loop de mensagens
                return 0;
            }
            case IDCANCEL:
                DestroyWindow(hDlg); // Fecha o diálogo
                PostQuitMessage(0);  // Finaliza o loop de mensagens
                return 0;
            }
            break;
    }

    return DefWindowProc(hDlg, message, wParam, lParam);
}

int ShowCustomDialog(HINSTANCE hInst, HWND hWndParent, char* entradaStr) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = CustomDialogProc;  // Associar a função de procedimento
    wc.hInstance = hInst;
    wc.lpszClassName = "CustomDialog";

    // Registra a classe da janela
    RegisterClass(&wc);

    // Cria a janela do diálogo
    HWND hDlg = CreateWindowEx(
        WS_EX_DLGMODALFRAME,
        "CustomDialog",            // Classe da janela
        "Entrada de Dados",        // Título da janela
        WS_POPUP | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 150,  // Posição e tamanho
        hWndParent, NULL, hInst, NULL);

    if (!hDlg) return 0;

    // Campo de texto
    CreateWindowEx(
        0, "EDIT", "",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        20, 40, 260, 20,
        hDlg, (HMENU)IDC_INPUT_FIELD, hInst, NULL);

    // Botão OK
    CreateWindowEx(
        0, "BUTTON", "OK",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        50, 80, 80, 25,
        hDlg, (HMENU)IDOK, hInst, NULL);

    // Botão Cancelar
    CreateWindowEx(
        0, "BUTTON", "Cancelar",
        WS_CHILD | WS_VISIBLE,
        150, 80, 80, 25,
        hDlg, (HMENU)IDCANCEL, hInst, NULL);

    // Exibe a janela do diálogo
    ShowWindow(hDlg, SW_SHOW);
    UpdateWindow(hDlg);

    // Loop de mensagens
    MSG msg;
    BOOL result;
    while ((result = GetMessage(&msg, NULL, 0, 0)) != 0) {
        if (result == -1) {
            break; // Erro na leitura das mensagens
        }

        // Verifica se a mensagem é para o diálogo
        if (!IsDialogMessage(hDlg, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 1; // Retorna sucesso
}

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
            lvc.cx = totalWidth * 0.07;
            ListView_InsertColumn(g_hListView, 0, &lvc);
            
            lvc.pszText = "Rótulo";
            lvc.cx = totalWidth * 0.09;
            ListView_InsertColumn(g_hListView, 1, &lvc);

            lvc.pszText = "Instrução";
            lvc.cx = totalWidth * 0.18;
            ListView_InsertColumn(g_hListView, 2, &lvc);
            
            lvc.pszText = "Atributo 1";
            lvc.cx = totalWidth * 0.18;
            ListView_InsertColumn(g_hListView, 3, &lvc);
            
            lvc.pszText = "Atributo 2";
            lvc.cx = totalWidth * 0.18;
            ListView_InsertColumn(g_hListView, 4, &lvc);
            
            lvc.pszText = "Comentário";
            lvc.cx = totalWidth * 0.29;
            ListView_InsertColumn(g_hListView, 5, &lvc);
            
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
                "Mode de Execução:", 
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
                hwnd, (HMENU)ID_NORMAL_RADIO, g_hInst, NULL
            );
            
            g_hStepByStepRadio = CreateWindowEx(
                0,
                "BUTTON",
                "Passo a Passo",
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                passoX, passoY, passoWidth, passoHeight,
                hwnd, (HMENU)ID_STEP_RADIO, g_hInst, NULL
            );
            
            // Create Execute and Stop buttons
            g_hExecuteButton = CreateWindowEx(
                0,
                "BUTTON",
                "Executar",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                executarX, executarY, executarWidth, executarHeight,
                hwnd, (HMENU)ID_EXECUTE_BUTTON, g_hInst, NULL
            );
            
            g_hStopButton = CreateWindowEx(
                0,
                "BUTTON",
                "Parar",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                pararX, pararY, pararWidth, pararHeight,
                hwnd, (HMENU)ID_STOP_BUTTON, g_hInst, NULL
            );


            // Set default radio button
            SendMessage(g_hStepByStepRadio, BM_SETCHECK, BST_CHECKED, 0);
            
            return 0;
        }
        
        case WM_NOTIFY: {
            LPNMHDR pNMHDR = (LPNMHDR)lParam;
            if (pNMHDR->hwndFrom == g_hListView && pNMHDR->code == NM_CUSTOMDRAW) {
                LPNMLVCUSTOMDRAW pLVCD = (LPNMLVCUSTOMDRAW)pNMHDR;

                switch (pLVCD->nmcd.dwDrawStage) {
                case CDDS_PREPAINT:
                    return CDRF_NOTIFYITEMDRAW;
                case CDDS_ITEMPREPAINT: {
                    if ((int)pLVCD->nmcd.dwItemSpec == countres) {
                        pLVCD->clrTextBk = RGB(240,230,140); // Fundo verde
                        pLVCD->clrText = RGB(0, 0, 0); // Texto branco
                    } else {
                        pLVCD->clrTextBk = RGB(255, 255, 255); // Fundo branco
                        pLVCD->clrText = RGB(0, 0, 0); // Texto preto
                    }
                    return CDRF_NEWFONT;
                }
                }
            }
            break;
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
                    case ID_NORMAL_RADIO:
                        if (HIWORD(wParam) == BN_CLICKED)
                        {
                            g_executionMode = 0; // Modo normal
                        }
                        break;

                    case ID_STEP_RADIO:
                        if (HIWORD(wParam) == BN_CLICKED)
                        {
                            g_executionMode = 1; // Modo passo a passo
                        }
                        break;

                    case ID_EXECUTE_BUTTON:
                        if (HIWORD(wParam) == BN_CLICKED)
                        {
                            ExecutarPrograma();
                            
                            // Se estiver no modo normal, desabilitar o botão após a execução
                            if (g_executionMode == 0)
                            {
                                EnableWindow(g_hExecuteButton, FALSE);
                            }
                        }
                        break;

                    case ID_STOP_BUTTON:
                        OutputDebugString("apertou o botao de parar\n");
                        if (HIWORD(wParam) == BN_CLICKED)
                        {
                            PararExecucao();
                            EnableWindow(g_hExecuteButton, TRUE); // Reabilita o botão de execução
                        }
                        break;

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
                             
                                p = inicializarPilha(PILHA_TAM_INICIAL);
                                if(p == NULL){
                                    return 1;
                                }
                                countres = 0;
                                lerInstrucoes(file);

                            }

                            fclose(file);
                        }
                        break;
                    }
                    case 1001: // ID para "Sair"
                        PostMessage(hwnd, WM_CLOSE, 0, 0);
                        break;
                }
            break;
            
        case WM_DESTROY:
            if (p != NULL) {
                liberarPilha();
                p = NULL;
            }
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                  VMD
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------


Pilha* inicializarPilha(int capacidadeInicial) {
    if(capacidadeInicial <=0){
        return NULL;
    }

    Pilha *p = (Pilha*)calloc(1, sizeof(Pilha));
    if (p == NULL) {
        printf("Erro: Falha na alocação da pilha\n");
        return NULL;
    }

    p->M = calloc(capacidadeInicial, sizeof(int));
    if (p->M == NULL) {
        free(p);
        printf("Erro ao alocar memória para a pilha.\n");
        exit(1);
    }
    
    p->s = -1;
    p->capacidade = capacidadeInicial;
    p->max_capacidade = PILHA_TAM_MAXIMO;
    return p;
}

int pilhaVazia() {
    return p->s == -1;
}

int pilhaCheia() {
    return p->s == p->capacidade - 1;
}

/// @brief s = s+1; M[s] = atributo 
int empilhar(int atributo) {
    if (!p || !p->M) return 0;

    if (pilhaCheia(p)) {
        if(redimensionarPilha(p)){
            return 0;
        }
    }

    p->M[++(p->s)] = atributo;
    return 1;
}
 
/// @brief  retorna M[s] ; s = s-1
int desempilhar(int *valor) {
    if (!p || !p->M || !valor || p->s < 0) return 0;
    
    *valor = p->M[p->s--];
    return 1;
}

/// @brief retorna M[s] 
int topo() {
    if (pilhaVazia(p)) {
        printf("Erro: Pilha vazia!\n");
        return -1;
    } else {
        return p->M[p->s];
    }
}

int redimensionarPilha() {
    if (!p || !p->M) return 0;
    
    if (p->capacidade >= p->max_capacidade) {
        return 0; // Evita crescimento excessivo
    }

    int novaCapacidade = p->capacidade * 2;
    if (novaCapacidade > p->max_capacidade) {
        novaCapacidade = p->max_capacidade;
    }

    int *novo_array = (int*)realloc(p->M, novaCapacidade * sizeof(int));
    if (!novo_array) {
        return 0;
    }

    p->M = novo_array;
    p->capacidade = novaCapacidade;
    return 1;
}

void liberarPilha() {
    if(p == NULL){
        return;
    }
    free(p->M);
    free(p);
}

int analisaInst(struct Inst *lista) {
    if (strcmp(lista->instrucao, "NULL") == 0)
        return 0;
    if (strcmp(lista->instrucao, "LDC     ") == 0)
        return 1;
    else if (strcmp(lista->instrucao, "LDV     ") == 0)
        return 2;
    else if (strcmp(lista->instrucao, "ADD     ") == 0)
        return 3;
    else if (strcmp(lista->instrucao, "SUB     ") == 0)
        return 4;
    else if (strcmp(lista->instrucao, "MULT    ") == 0)
        return 5;
    else if (strcmp(lista->instrucao, "DIVI    ") == 0)
        return 6;
    else if (strcmp(lista->instrucao, "INV     ") == 0)
        return 7;
    else if (strcmp(lista->instrucao, "AND     ") == 0)
        return 8;
    else if (strcmp(lista->instrucao, "OR      ") == 0)
        return 9;
    else if (strcmp(lista->instrucao, "NEG     ") == 0)
        return 10;
    else if (strcmp(lista->instrucao, "CME     ") == 0)
        return 11;
    else if (strcmp(lista->instrucao, "CMA     ") == 0)
        return 12;
    else if (strcmp(lista->instrucao, "CEQ     ") == 0)
        return 13;
    else if (strcmp(lista->instrucao, "CDIF    ") == 0)
        return 14;
    else if (strcmp(lista->instrucao, "CMEQ    ") == 0)
        return 15;
    else if (strcmp(lista->instrucao, "CMAQ    ") == 0)
        return 16;
    else if (strcmp(lista->instrucao, "JMP     ") == 0)
        return 17;
    else if (strcmp(lista->instrucao, "JMPF    ") == 0)
        return 18;
    else if (strcmp(lista->instrucao, "STR     ") == 0)
        return 19;
    else if (strcmp(lista->instrucao, "RD      ") == 0)
        return 20;
    else if (strcmp(lista->instrucao, "PRN     ") == 0)
        return 21;
    else if (strcmp(lista->instrucao, "START   ") == 0)
        return 22;
    else if (strcmp(lista->instrucao, "ALLOC   ") == 0)
        return 23;
    else if (strcmp(lista->instrucao, "DALLOC  ") == 0)
        return 24;
    else if (strcmp(lista->instrucao, "CALL    ") == 0)
        return 25;
    else if (strcmp(lista->instrucao, "RETURN  ") == 0)
        return 26;
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
    //AtualizarListViewMemoria();
}

void resolveInst(int* count){
    if (!count || *count < 0 || *count >= MAX_INST) return;
    int m = 0;
    int n = 0;
    int l = 0; // temporario
    int resultado = 0;
    switch (analisaInst(&lista[*count])) {
        case 0: // NULL
            // Nada
            break;
        case 1: // LDC k (Carregar constante)
            empilhar(atoi(lista[*count].atr1));
            break;

        case 2: // LDV n (Carregar variavel)
            empilhar(p->M[atoi(lista[*count].atr1)]);
            break;

        case 3: // ADD (Somar)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            resultado = n + m; // m[s-1] + m[s]
            p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            break;

        case 4: // SUB (Subtrair)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            resultado = n - m; // m[s-1] - m[s]
            p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            break;

        case 5: // MULT (Multiplicar)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            resultado = n * m; // m[s-1] * m[s]
            p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            break;

        case 6: // DIVI (Dividir)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            if (m != 0){
                resultado = n / m; // m[s-1] div m[s]
                p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            }
            else
                printf("Erro: Divisão por zero!\n");
            break;

        case 7: // INV (Inverter sinal)
            p->M[p->s] = -(p->M[p->s]);
            break;

        case 8: // AND (Conjunção)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            resultado = n && m; // m[s-1] and m[s]
            p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            break;

        case 9: // OR (Disjunção)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            resultado = n || m; // m[s-1] OU m[s]
            p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            break;
            
        case 10: // NEG (Negação)
            p->M[p->s] = 1 - p->M[p->s];
            break;

        case 11: // CME (Comparar menor)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            resultado = n < m; // m[s-1] < m[s]
            char* teste;
            p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            break;

        case 12: // CMA (Comparar maior)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            resultado = n > m; // m[s-1] > m[s]
            p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            break;

        case 13: // CEQ (Comparar igual)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            resultado = n == m; // m[s-1] = m[s]
            p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            break;

        case 14: // CDIF (Comparar desigual)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            resultado = n != m; // m[s-1] != m[s]
            p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            break;

        case 15: // CMEQ (Comparar menor ou igual)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            resultado = n <= m; // m[s-1] <= m[s]
            p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            break;

        case 16: // CMAQ (Comparar maior ou igual)
            if (!desempilhar(&m) || !desempilhar(&n)) return;
            resultado = n >= m; // m[s-1] >= m[s]
            p->M[++(p->s)] = resultado; //m[s-1] = res; s = s-1
            break;
        
        case 17: // JMP p (Desviar sempre)
            for (int i = -1; i < MAX_INST; i++) {
                // Verifique se o rótulo da linha atual é igual a lista[count].atr1
                if (strcmp(lista[i].rotulo, lista[*count].atr1) == 0) {
                    printf("\n rotulo = %s, instrucao = %s, atr1 = %s",lista[i].rotulo, lista[*count].instrucao, lista[*count].atr1);
                    // Se for igual, você pode executar a ação desejada
                    *count = i;
                    return;
                    // Faça o que for necessário quando o rótulo for encontrado
                }
            };
            break;

        case 18: // JMPF p (Desviar se falso)
            desempilhar(&l);
            if(l == 0){
                for (int i = -1; i < MAX_INST; i++) {
                    // Verifique se o rótulo da linha atual é igual a lista[count].atr1
                    if (strcmp(lista[i].rotulo, lista[*count].atr1) == 0) {
                        // Se for igual, você pode executar a ação desejada
                        *count = i;
                        return;
                        // Faça o que for necessário quando o rótulo for encontrado
                    }
                }
            };
            break;

        case 19: // STR n (Armazenar valor)
            p->M[atoi(lista[*count].atr1)] = p->M[p->s];
            (p->s)--;
            break;
 
        case 20: // RD (Leitura)
            /*//printf("Digite o próximo valor de entrada: ");
            int entrada;
            fflush(stdin);
            scanf("%d", &entrada);
            printf("\n Entrada = %d", entrada);
            empilhar(entrada);*/
            char entradaStr[256] = "";
            if (ShowCustomDialog(g_hInst, NULL, entradaStr)) {
                printf("Entrada emilhada\n");
            } else {
                printf("Entrada cancelada pelo usuário.\n");
            }
            break;

        case 21: // PRN (Impressão)                
            char buffer[12];
            char *currentText;
            DWORD textLen;
            
            desempilhar(&l);
            itoa(l, buffer, 10);
            
            // Obter o tamanho do texto atual
            textLen = GetWindowTextLength(g_hOutputEdit);
            
            // Alocar memória suficiente para o texto atual + nova linha + novo número + terminador nulo
            DWORD len = textLen + strlen(buffer) + 3;
            currentText = (char*)calloc(1, len); // +3 para \r\n e \0
            if (currentText) {
                // Obter o texto atual
                GetWindowText(g_hOutputEdit, currentText, textLen + 1);
                
                // Se já existe texto, adicionar uma nova linha
                if (textLen > 0) {
                    strcat(currentText, "\r\n");
                }
                
                // Adicionar o novo número
                strcat(currentText, buffer);
                
                // Definir o novo texto
                SetWindowText(g_hOutputEdit, currentText);
                
                // Liberar a memória alocada
                free(currentText);
                
                // Rolar para mostrar a última linha
                SendMessage(g_hOutputEdit, EM_SCROLLCARET, 0, 0);
            }
            break;

        case 23: // ALLOC (Alocar memória)
            n = atoi(lista[*count].atr2);
            for (int k = 0; k < n; k++){
                m = atoi(lista[*count].atr1);
                empilhar(p->M[m+k]);
            }
            break;

        case 24: // DALLOC m, n (Liberar memória)
            n = atoi(lista[*count].atr2);
            m = atoi(lista[*count].atr1);
            //MessageBoxW(hwnd, L"Entrou no DALLOC.", L"Erro", MB_OK | MB_ICONERROR);
            for (int k = (n - 1); k >= 0; k--) {
                desempilhar(&l);
                p->M[m+k] = l;                 // Decrementa o topo da pilha
            }
            break;

        case 25: // CALL p (Chamar procedimento ou função)
            empilhar(((*count)+1));
            for (int i = 0; i < MAX_INST; i++) {
                
                // Verifique se o rótulo da linha atual é igual a lista[count].atr1
                if (strcmp(lista[i].rotulo, lista[*count].atr1) == 0) {
                    // Se for igual, você pode executar a ação desejada
                    *count = i;
                    return;
                    // Faça o que for necessário quando o rótulo for encontrado
                }
            }
            break;

        case 26: // Return
            desempilhar(&l);
            *count = l -1;

        default:
            printf("Instrução inválida: \n");
            break;
    }
}

// Início da MVD
void MVD() {
    if (strcmp(lista[0].instrucao, "START   ") != 0) {
    MessageBoxW(hwnd, L"A Primeira instrução deve ser START", L"ERRO", MB_OK | MB_ICONINFORMATION);
        return;
    }
    while (strcmp(lista[countres].instrucao, "HLT     ") != 0) {
            countres++;
            resolveInst(&countres);
    }
    MessageBoxW(hwnd, L"Programa finalizado", L"Informação", MB_OK | MB_ICONINFORMATION);
    
    if (p != NULL) {
        liberarPilha();
        p = NULL;
    }
}