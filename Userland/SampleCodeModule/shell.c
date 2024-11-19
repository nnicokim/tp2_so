#include <shell.h>
#include <interrupts.h>
#include <songs.h>
#include "./include/tests/test_prio.h"
#include "./include/phylo.h"

// #define INPUT_SIZE 50
#define CANT_REGS 18
#define MAX_PROCESS 192
#define TRUE 1
#define RUNNING 1

void help();
void divzero();
void invopcode();
void time();
void play_eliminator();
void zoomin();
void zoomout();
void inforeg();
void clear_shell();
void beep();
void victory();
void mario_bros_song();
void easteregg();
void sh_test_processes();
void sh_test_mm();
void sh_test_prio();
void print_processes();
void print_memory();
void create_one_process();
void loop_print();
void kill_process_pid();
void block_process_pid();
void unblock_process_pid();
void increase_prio_pid();
void decrease_prio_pid();
void nice_pid();
void sh_test_sync1();
void sh_test_sync2();
void handleCommands(char *str, int *fd);
void handleRegularCommand(char *str, int *fd);
void wc(char **params);
void cat(char **params);
void filter(char **params);
void phylos();


static char buffer[INPUT_SIZE] = {0};
static int bufferIndex = 0;
static int currentFontSize;
static int gameActive = 0;
int fgFD[] = {0, 1};
int bgFD[] = {-1, -1};

static Command commands[] = {
    {"help", help, "Muestra la lista de comandos"},
    {"divzero", divzero, "Simula la excepcion de division por 0"},
    {"invopcode", invopcode, "Simula la excepcion de opcode invalida"},
    {"time", time, "Muestra la hora actual"},
    {"inforeg", inforeg, "Imprime los registros capturados por CTRL"},
    {"eliminator", play_eliminator, "Inicia el juego de eliminator"},
    {"zoomin", zoomin, "Aumenta el tamanio de la letra"},
    {"zoomout", zoomout, "Disminuye el tamanio de la letra"},
    {"clear", clear_shell, "Limpia la shell"},
    {"beep", beep, "Emite un beep"},
    {"victory", victory, "VAMOS CARAJO!!!"},
    {"mario", mario_bros_song, "Canta el himno de Mario Bros"},
    {"tpr", sh_test_processes, "Testea los procesos"},
    {"tmm", sh_test_mm, "Testea el gestor de memoria"},
    {"tprio", sh_test_prio, "Testea la prioridad de los procesos"},
    {"printp", print_processes, "Imprime los procesos activos"},
    {"loop", loop_print, "Imprime el PID del proceso ejecutandose cada 2 segs"},
    {"kill", kill_process_pid, "Mata un proceso dado un PID"},
    {"block", block_process_pid, "Bloquea un proceso dado un PID"},
    {"unblock", unblock_process_pid, "Desbloquea un proceso dado un PID"},
    {"incp", increase_prio_pid, "Cambia la prioridad de un proceso dado un PID y una prioridad"},
    {"decp", decrease_prio_pid, "Cambia la prioridad de un proceso dado un PID y una prioridad"},
    {"nice", nice_pid, "Cambia la prioridad de un proceso dado un PID y una prioridad"},
    {"wc", wc, "Cuenta la cantidad de saltos de linea."},
    {"cat", cat, "Imprime el contenido de un input."},
    {"filter", filter, "Filtra las vocales de un input."},
    {"phylo", phylos, "Inicia el problema de los filosofos"}
    };

static Command commandsNohelp[] = {
    {"cp", create_one_process, "Crea un proceso"},
    {"mem", print_memory, "Imprime la memoria"},
    {"tsync1", sh_test_sync1, "Testea la sincronizacion con semaforos"},
    {"tsync2", sh_test_sync2, "Testea la sincronizacion sin semaforos"},
    {"egg", easteregg, "Easter egg song"},
    {"rick", playRick, "Rick Astley"}};

#define sizeofArr(arr) (sizeof(arr) / sizeof(arr[0]))
#define COMMAND_COUNT sizeofArr(commands)

void parseCommand(char *str)
{
    // char argument[] = {0};
    if (strcmp_u(str, "") == 0)
    {
        return;
    }

    if (str[strlen_u(str) - 2] == '&')
    {
        str[strlen_u(str) - 1] = '\0';
        str[strlen_u(str) - 2] = '\0';
        return handleCommands(str, bgFD);
    }

    return handleCommands(str, fgFD);
}

void handleRegularCommand(char *str, int *fd)
{
    int argC = 0;
    char *argument[] = {0};

    if (fd == bgFD)
    {
        str[strlen_u(str) - 2] = '\0';
        print(str);
        putChar('\n');
        for (int i = 0; i < COMMAND_COUNT; i++)
        {
            if (strcmp_u(str, commands[i].name_id) == 0)
            {
                printColor(ORANGE, "Creating background process\n");
                usys_createProcess(commands[i].name_id, commands[i].func, argC, argument, fd);
                // printPromptIcon();
                return;
            }
        }
    }

    // int argC = parseCommandArg(str); // Esto me da los espacios entre los comandos

    while (*str == ' ')
        str++;

    for (int i = 0; i < COMMAND_COUNT; i++)
    {
        if (strcmp_u(str, commands[i].name_id) == 0)
        {
            int pid = usys_createProcess(str, commands[i].func, argC, argument, fd);
            usys_waitPid(pid);
            return;
        }

        if (strcmp_u(str, commandsNohelp[i].name_id) == 0)
        {
            int pid = usys_createProcess(str, commandsNohelp[i].func, argC, argument, fd);
            usys_waitPid(pid);
            return;
        }
    }
    printError("Error: comando no diponible. Ingrese \"help\" para ver los comandos disponibles.\n");
}

void handleCommands(char *str, int *fd)
{
    char *cmds[3] = {str, 0};
    char *currCMD = str;
    uint64_t currentID = 0;
    while (*currCMD != '\0' && *currCMD != '\n' && currentID < 3)
    {
        if (*currCMD == '|')
        {
            *currCMD = '\0'; // Terminate current command
            currentID++;     // Move to the next command
            if (currentID < 3)
            {                                  // Check bounds before assignment
                cmds[currentID] = currCMD + 1; // Set the next command start
            }
        }
        currCMD++;
    }

    if (currentID >= 3)
    {
        printError("Demasiados comandos en la linea de comandos.\n");
        return;
    };

    for (int i = 0; i <= currentID; i++)
        handleRegularCommand(cmds[i], fd);
}

void printPromptIcon()
{
    printColor(GREEN, "$>");
}

void init_shell()
{
    print("Bienvenido a ");
    printColor(GREEN, "OUR_OS\n");
    print("Ingrese uno de los siguientes comandos:\n");
    for (int i = 1; i < COMMAND_COUNT - 1; i++)
    {
        printColor(LIGHT_BLUE, commands[i].name_id);
        print(" | ");
    }
    printColor(LIGHT_BLUE, commands[COMMAND_COUNT - 1].name_id);
    print("\nIngrese ");
    printColor(PURPLE, "\"help\" ");
    print("para la descripcion los comandos.\n");

    char c;
    currentFontSize = usys_get_font_size();
    printPromptIcon();
    while (RUNNING)
    {                  // if ESC
        c = getChar(); // non-blocking read
        if (c != 0)
        {
            if (c == '\b' && bufferIndex > 0)
            {
                bufferIndex--;
                putChar(c);
            }
            else if (c == '\n')
            {
                putChar(c);
                buffer[bufferIndex] = '\0'; // Null-terminate the command
                parseCommand(buffer);       // Process the command
                bufferIndex = 0;            // Reset the buffer for the next command
                printPromptIcon();
            }
            else if (c != '\b')
            {
                putChar(c);
                buffer[bufferIndex++] = c;
            }
        }
    }
}

void help()
{
    printColor(ORANGE, "Comandos disponibles:\n");
    for (int i = 1; i < COMMAND_COUNT; i++)
    {
        print("   ");
        printColor(LIGHT_BLUE, commands[i].name_id);
        print(": ");
        print(commands[i].desc);
        putChar('\n');
    }
    usys_myExit();
}

void divzero()
{
    int a = 1;
    int b = 0;
    if (a / b == 1)
    {
        printError("This is wrong...");
    }
    usys_myExit();
}
void invopcode()
{
    _invalid_opcode_exception();
    usys_myExit();
}

void time()
{
    printColor(GREEN, "ART (Argentine Time): UTC/GMT -3 horas\n");
    usys_get_time();
    usys_myExit();
}

void zoomin()
{
    printColor(YELLOW, "Esto va a borrar toda la pantalla. Esta seguro de que quiere proceder?\n");
    printColor(ORANGE, "Indicar Si (Si) o N (No)\n");
    char c;
    while (1)
    {
        while ((c = getChar()) == 0)
            ;
        if (c == 'N' || c == 'n')
        {
            usys_myExit();
            return;
        }
        else if (c == 'S' || c == 's')
        {
            if (currentFontSize >= 3)
            {
                printColor(RED, "No se puede agrandar mas.\n");
                usys_myExit();
                return;
            }
            usys_change_font_size(++currentFontSize);
            clear_shell();
            usys_myExit();
            return;
        }
        else
        {
            printColor(RED, "Indique S o N\n");
        }
    }

    usys_myExit();
}

void zoomout()
{
    printColor(YELLOW, "Esto va a borrar toda la pantalla. Esta seguro de que quiere proceder?\n");
    printColor(ORANGE, "Indicar S (Si) o N (No)\n");
    char c;
    while (1)
    {
        while ((c = getChar()) == 0)
            ;
        if (c == 'N' || c == 'n')
        {
            usys_myExit();
            return;
        }
        else if (c == 'S' || c == 's')
        {
            if (currentFontSize <= 1)
            {
                printColor(RED, "No se puede achicar mas.\n");
                usys_myExit();
                return;
            }
            usys_change_font_size(--currentFontSize);
            clear_shell();
            usys_myExit();
            return;
        }
        else
        {
            printColor(RED, "Indique S o N\n");
        }
    }

    usys_myExit();
}

void inforeg()
{
    uint64_t regs[CANT_REGS];
    char *regsNames[CANT_REGS] = {"rax:", "rbx:", "rcx:", "rdx:", "rsi:", "rdi:", "rbp:", "rsp:", "r8:", "r9:",
                                  "r10:", "r11:", "r12:", "r13:", "r14:", "r15:", "rip:", "rflags:"};
    char aux[128];
    int flag = usys_getRegisters(regs);
    if (flag)
    {
        printColor(GREEN, "Informacion de los registros: \n");
        for (int i = 0; i < CANT_REGS; i++)
        {
            print("    ");
            printColor(GREEN, regsNames[i]);
            uintToBase(regs[i], aux, 16);
            print(aux);
            putChar('\n');
        }
    }
    else
    {
        printColor(RED, "No se pudo encontrar ningun momento de captura de registro. Presione CTRL para capturar los registros.\n");
    }
    usys_myExit();
}

void clear_shell()
{
    usys_clear_screen();
    usys_myExit();
}

void beep()
{
    printColor(GREEN, "BEEP!!\n");
    usys_beep(500, 10);
    usys_myExit();
}

void victory() // Ejecutar despues de cada victoria
{
    printColor(GREEN, "Paso el Test!!!!!!!\n");
    usys_beep(659, 50);
    usys_beep(659, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(167);
    usys_beep(523, 50);
    usys_beep(659, 50);
    usys_wait(50);
    usys_beep(784, 50);
    usys_wait(375);
    usys_beep(392, 50);
    usys_wait(375);
    usys_beep(523, 50);
    usys_wait(250);
    usys_beep(392, 50);
    usys_wait(250);
    usys_beep(330, 50);
    usys_wait(250);
    usys_beep(440, 50);
    usys_wait(50);
    usys_beep(494, 50);
    usys_wait(50);
    usys_beep(466, 50);
    usys_wait(42);
    usys_beep(440, 50);
    usys_wait(50);
    usys_beep(392, 50);
    usys_wait(50);
    usys_myExit();
}

void mario_bros_song()
{
    // printColor(GREEN, "Mario Bros\n");

    usys_beep(659, 50);
    usys_beep(659, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(167);
    usys_beep(523, 50);
    usys_beep(659, 50);
    usys_wait(50);
    usys_beep(784, 50);
    usys_wait(375);
    usys_beep(392, 50);
    usys_wait(375);
    usys_beep(523, 50);
    usys_wait(250);
    usys_beep(392, 50);
    usys_wait(250);
    usys_beep(330, 50);
    usys_wait(250);
    usys_beep(440, 50);
    usys_wait(50);
    usys_beep(494, 50);
    usys_wait(50);
    usys_beep(466, 50);
    usys_wait(42);
    usys_beep(440, 50);
    usys_wait(50);
    usys_beep(392, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(50);
    usys_beep(784, 50);
    usys_wait(50);
    usys_beep(880, 50);
    usys_wait(50);
    usys_beep(698, 50);
    usys_beep(784, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(50);
    usys_beep(523, 50);
    usys_wait(50);
    usys_beep(587, 50);
    usys_beep(494, 50);
    usys_wait(50);
    usys_beep(523, 50);
    usys_wait(250);
    usys_beep(392, 50);
    usys_wait(250);
    usys_beep(330, 50);
    usys_wait(250);
    usys_beep(440, 50);
    usys_wait(50);
    usys_beep(494, 50);
    usys_wait(50);
    usys_beep(466, 50);
    usys_wait(42);
    usys_beep(440, 50);
    usys_wait(50);
    usys_beep(392, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(50);
    usys_beep(784, 50);
    usys_wait(50);
    usys_beep(880, 50);
    usys_wait(50);
    usys_beep(698, 50);
    usys_beep(784, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(50);
    usys_beep(523, 50);
    usys_wait(50);
    usys_beep(587, 50);
    usys_beep(494, 50);
    usys_wait(375);
    usys_beep(784, 50);
    usys_beep(740, 50);
    usys_beep(698, 50);
    usys_wait(42);
    usys_beep(622, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(167);
    usys_beep(415, 50);
    usys_beep(440, 50);
    usys_beep(523, 50);
    usys_wait(50);
    usys_beep(440, 50);
    usys_beep(523, 50);
    usys_beep(587, 50);
    usys_wait(250);
    usys_beep(784, 50);
    usys_beep(740, 50);
    usys_beep(698, 50);
    usys_wait(42);
    usys_beep(622, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(167);
    usys_beep(698, 50);
    usys_wait(50);
    usys_beep(698, 50);
    usys_beep(698, 50);
    usys_wait(625);
    usys_beep(784, 50);
    usys_beep(740, 50);
    usys_beep(698, 50);
    usys_wait(42);
    usys_beep(622, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(167);
    usys_beep(415, 50);
    usys_beep(440, 50);
    usys_beep(523, 50);
    usys_wait(50);
    usys_beep(440, 50);
    usys_beep(523, 50);
    usys_beep(587, 50);
    usys_wait(250);
    usys_beep(622, 50);
    usys_wait(250);
    usys_beep(587, 50);
    usys_wait(250);
    usys_beep(523, 50);
    usys_wait(130);
    usys_beep(784, 50);
    usys_beep(740, 50);
    usys_beep(698, 50);
    usys_wait(42);
    usys_beep(622, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(167);
    usys_beep(415, 50);
    usys_beep(440, 50);
    usys_beep(523, 50);
    usys_wait(50);
    usys_beep(440, 50);
    usys_beep(523, 50);
    usys_beep(587, 50);
    usys_wait(250);
    usys_beep(784, 50);
    usys_beep(740, 50);
    usys_beep(698, 50);
    usys_wait(42);
    usys_beep(622, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(167);
    usys_beep(698, 50);
    usys_wait(50);
    usys_beep(698, 50);
    usys_beep(698, 50);
    usys_wait(625);
    usys_beep(784, 50);
    usys_beep(740, 50);
    usys_beep(698, 50);
    usys_wait(42);
    usys_beep(622, 50);
    usys_wait(50);
    usys_beep(659, 50);
    usys_wait(167);
    usys_beep(415, 50);
    usys_beep(440, 50);
    usys_beep(523, 50);
    usys_wait(50);
    usys_beep(440, 50);
    usys_beep(523, 50);
    usys_beep(587, 50);
    usys_wait(230);
    usys_beep(622, 50);
    usys_wait(250);
    usys_beep(587, 50);
    usys_wait(230);
    usys_beep(523, 50);
    usys_wait(150);
    usys_beep(523, 50);
    usys_myExit();
}

void easteregg()
{
    printColor(GREEN, "Guess who's symphony...\n");

    usys_beep(523, 250);
    usys_wait(90);
    usys_beep(523, 250);
    usys_wait(90);
    usys_beep(523, 250);
    usys_wait(800);
    usys_beep(415, 750);
    usys_wait(200);

    usys_beep(466, 250);
    usys_wait(100);
    usys_beep(466, 250);
    usys_wait(100);
    usys_beep(466, 250);
    usys_wait(100);
    usys_beep(392, 750);
    usys_wait(200);

    usys_beep(523, 250);
    usys_wait(100);
    usys_beep(523, 250);
    usys_wait(100);
    usys_beep(523, 250);
    usys_wait(100);
    usys_beep(415, 750);
    usys_wait(200);

    usys_beep(466, 250);
    usys_wait(80);
    usys_beep(466, 250);
    usys_wait(80);
    usys_beep(466, 250);
    usys_wait(80);
    usys_beep(392, 750);
    usys_beep(392, 750);

    printColor(TURQUOISE, "Encontraron el easter egg oculto!!!\n");
    usys_myExit();
}

void play_eliminator()
{
    gameActive = 1;
    clear_shell();
    eliminator();
    clear_shell();
    gameActive = 0;
    usys_myExit();
}

void sh_test_processes()
{
    char *argvAux[] = {"10"};
    test_processes(1, argvAux);
    usys_myExit();
}

void sh_test_prio()
{
    printColor(ORANGE, "Testeando prioridades...\n");
    test_prio();
    usys_myExit();
}

void unblock_process_pid(){
    printColor(ORANGE, "Ingrese el PID del proceso a desbloquear: ");
    char pid[5] = {"0"};
    int i = 0;
    char c;
    int unblock_pid;
    while (TRUE)
    {
        while (TRUE)
        {
            c = getChar();
            if (c != 0)
            {
                putChar(c);
                if ((c < '0' || c > '9') && c != '\n')
                {
                    putChar('\n');
                    printColor(RED, "ERROR. Ingrese un digito valido.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (i > 3)
                {
                    putChar('\n');
                    printColor(RED, "ERROR. PID muy largo.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (c == '\n')
                {
                    unblock_pid = stringToInt(pid);
                    break;
                }
                pid[i++] = c;
            }
        }

        if (unblock_pid < 0 || unblock_pid > MAX_PROCESS)
        {
            printColor(RED, "PID invalido. Ingrese un PID valido.\n");
            usys_myExit();
            return;
        }
        if (unblock_pid == 0 || unblock_pid == 1)
        {
            printColor(RED, "No se puede desbloquear el proceso SHELL o IDLE. Ingrese otro PID.\n");
            usys_myExit();
            return;
        }

        printColor(ORANGE, "Desbloqueando al proceso...\n");
        int resultado = usys_unblockProcess(unblock_pid);   
        if (resultado == -1)
        {
            printColor(RED, "No se pudo desbloquear al proceso con PID: ");
            intToStr(unblock_pid, pid);
            print(pid);
            putChar('\n');
            usys_myExit();
            return;
        }
        else
        {
            printColor(GREEN, "Se desbloqueo al proceso con PID: ");
            intToStr(unblock_pid, pid);
            print(pid);
            putChar('\n');
            usys_myExit();
            return;
        }
    }
    usys_myExit();
}

void sh_test_mm()
{
    printColor(ORANGE, "Testeando MM...\n");
    char *argvmm[] = {"500"};
    test_mm(1, argvmm);
    usys_myExit();
}

void print_processes()
{
    printColor(ORANGE, "Imprimiendo procesos...\n");
    usys_print_processes();
    usys_myExit();
}

void print_memory()
{
    printColor(ORANGE, "Imprimiendo memoria...\n");
    usys_print_memory();
    usys_myExit();
}

void create_one_process()
{
    printColor(ORANGE, "Creando un proceso...\n");
    usys_createOneProcess();
    usys_myExit();
}

void loop_print()
{
    printColor(ORANGE, "Imprimiendo el PID de un proceso cada 2 segundos...\n");
    usys_loop_print();
    usys_myExit();
}

void sh_test_sync1()
{
    printColor(ORANGE, "Testeando sincronizacion con sincro...\n");
    char *argv1[] = {"10", "1", "0"};
    test_sync(3, argv1);
    usys_myExit();
}

void sh_test_sync2()
{
    printColor(ORANGE, "Testeando sincronizacion sin sincro...\n");
    char *argv2[] = {"10", "1", "1"};
    test_sync(3, argv2);
    usys_myExit();
}

void kill_process_pid()
{
    printColor(ORANGE, "Ingrese el PID del proceso a matar: ");
    char pid[5] = {"0"};
    int i = 0;
    char c;
    int kill_pid;
    while (TRUE)
    {
        while (TRUE)
        {
            c = getChar();
            if (c != 0)
            {
                putChar(c);
                if ((c < '0' || c > '9') && c != '\n')
                {
                    putChar('\n');
                    printColor(RED, "ERROR. Ingrese un digito valido.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (i > 3)
                {
                    putChar('\n');
                    printColor(RED, "ERROR. PID muy largo.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (c == '\n')
                {
                    kill_pid = stringToInt(pid);
                    break;
                }
                pid[i++] = c;
            }
        }

        if (kill_pid < 0 || kill_pid > MAX_PROCESS)
        {
            print("\n");
            printColor(RED, "PID invalido. Ingrese un PID valido.\n");
            usys_myExit();
            return;
        }
        if (kill_pid == 0 || kill_pid == 1)
        {
            putChar('\n');
            printColor(RED, "No se puede matar el proceso SHELL o IDLE. Ingrese otro PID.\n");
            usys_myExit();
            return;
        }

        printColor(ORANGE, "Matando al proceso...\n");
        int resultado = usys_killProcess(kill_pid);
        if (resultado == -1)
        {
            printColor(RED, "No se pudo matar al proceso con PID: ");
            intToStr(kill_pid, pid);
            print(pid);
            putChar('\n');
            usys_myExit();
            return;
        }
        else
        {
            printColor(GREEN, "Se mato al proceso con PID: ");
            intToStr(kill_pid, pid);
            print(pid);
            putChar('\n');
            usys_myExit();
            return;
        }
    }

    usys_myExit();
}

void block_process_pid()
{
    printColor(ORANGE, "Ingrese el PID del proceso a bloquear: ");
    char pid[5] = {"0"};
    int i = 0;
    char c;
    int block_pid;
    while (TRUE)
    {
        while (TRUE)
        {
            c = getChar();
            if (c != 0)
            {
                putChar(c);
                if ((c < '0' || c > '9') && c != '\n')
                {
                    putChar('\n');
                    printColor(RED, "ERROR. Ingrese un digito valido.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (i > 3)
                {
                    putChar('\n');
                    printColor(RED, "ERROR. PID muy largo.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (c == '\n')
                {
                    block_pid = stringToInt(pid);
                    break;
                }
                pid[i++] = c;
            }
        }

        if (block_pid < 0 || block_pid > MAX_PROCESS)
        {
            printColor(RED, "PID invalido. Ingrese un PID valido.\n");
            usys_myExit();
            return;
        }
        if (block_pid == 0 || block_pid == 1)
        {
            printColor(RED, "No se puede bloquear el proceso SHELL o IDLE. Ingrese otro PID.\n");
            usys_myExit();
            return;
        }

        printColor(ORANGE, "Bloqueando al proceso...\n");
        int resultado = usys_blockProcess(block_pid);
        if (resultado == -1)
        {
            printColor(RED, "No se pudo bloquear al proceso con PID: ");
            intToStr(block_pid, pid);
            print(pid);
            putChar('\n');
            usys_myExit();
            return;
        }
        else
        {
            printColor(GREEN, "Se bloqueo al proceso con PID: ");
            intToStr(block_pid, pid);
            print(pid);
            putChar('\n');
            usys_myExit();
            return;
        }
    }

    usys_myExit();
}

void increase_prio_pid()
{
    printColor(ORANGE, "Ingrese el PID del proceso a incrementar su prioridad: ");
    char pid[5] = {"0"};
    int i = 0;
    char c;
    int inc_pid;
    while (TRUE)
    {
        while (TRUE)
        {
            c = getChar();
            if (c != 0)
            {
                putChar(c);
                if ((c < '0' || c > '9') && c != '\n')
                {
                    putChar('\n');
                    printColor(RED, "ERROR. Ingrese un digito valido.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (i > 3)
                {
                    putChar('\n');
                    printColor(RED, "ERROR. PID muy largo.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (c == '\n')
                {
                    inc_pid = stringToInt(pid);
                    break;
                }
                pid[i++] = c;
            }
        }

        if (inc_pid < 0 || inc_pid > MAX_PROCESS)
        {
            printColor(RED, "PID invalido. Ingrese un PID valido.\n");
            usys_myExit();
            return;
        }

        printColor(ORANGE, "Aumentando la prioridad del proceso...\n");
        int resultado = usys_increase_priority(inc_pid);
        if (resultado == -1)
        {
            printColor(RED, "No se pudo incrementar la prioridad del proceso.\n");
            usys_myExit();
            return;
        }
        else
        {
            printColor(GREEN, "Se incremento la prioridad del proceso :) \n");
            usys_myExit();
            return;
        }
    }
}

void decrease_prio_pid()
{
    printColor(ORANGE, "Ingrese el PID del proceso a decrementar su prioridad: ");
    char pid[5] = {"0"};
    int i = 0;
    char c;
    int dec_pid;
    while (TRUE)
    {
        while (TRUE)
        {
            c = getChar();
            if (c != 0)
            {
                putChar(c);
                if ((c < '0' || c > '9') && c != '\n')
                {
                    putChar('\n');
                    printColor(RED, "ERROR. Ingrese un digito valido.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (i > 3)
                {
                    putChar('\n');
                    printColor(RED, "ERROR. PID muy largo.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (c == '\n')
                {
                    dec_pid = stringToInt(pid);
                    break;
                }
                pid[i++] = c;
            }
        }

        if (dec_pid < 0 || dec_pid > MAX_PROCESS)
        {
            printColor(RED, "PID invalido. Ingrese un PID valido.\n");
            usys_myExit();
            return;
        }

        printColor(ORANGE, "Decrementando la prioridad del proceso...\n");
        int resultado = usys_decrease_priority(dec_pid);
        if (resultado == -1)
        {
            printColor(RED, "No se pudo decrementar la prioridad del proceso.");
            putChar('\n');
            usys_myExit();
            return;
        }
        else
        {
            printColor(GREEN, "Se decremento la prioridad del proceso :) \n");
            putChar('\n');
            usys_myExit();
            return;
        }
    }

    usys_myExit();
}

void nice_pid()
{
    printColor(ORANGE, "Ingrese el PID del proceso y la nueva prioridad: ");
    char pid[5] = {"0"};
    char prio[5] = {"0"};
    int i = 0;
    char c;
    int nice_pid;
    int newPrio;
    int flag = 0;
    while (TRUE)
    {
        while (TRUE)
        {
            c = getChar();
            if (c != 0)
            {
                putChar(c);
                if ((c < '0' || c > '9') && c != '\n' && c != ' ')
                {
                    putChar('\n');
                    printColor(RED, "ERROR. Ingrese un digito valido.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (i > 3)
                {
                    putChar('\n');
                    printColor(RED, "ERROR. PID muy largo.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (c == ' ')
                {
                    flag = 1;
                    i = 0;
                    continue;
                }
                if (c == '\n')
                {
                    if (flag == 0)
                    {
                        putChar('\n');
                        printColor(RED, "ERROR. Ingrese un espacio entre el PID y la prioridad.\n");
                        printColor(YELLOW, "Vuelva a intentarlo.\n");
                        usys_myExit();
                        return;
                    }
                    nice_pid = stringToInt(pid);
                    newPrio = stringToInt(prio);
                    break;
                }
                if (flag == 0)
                {
                    pid[i++] = c;
                }
                else
                {
                    prio[i++] = c;
                }
            }
        }
        if (nice_pid < 0 || nice_pid > MAX_PROCESS)
        {
            printColor(RED, "PID invalido. Ingrese un PID valido.\n");
            usys_myExit();
            return;
        }
        if (newPrio < 0 || newPrio > 5)
        {
            printColor(RED, "Prioridad invalida. Ingrese una prioridad valida.\n");
            usys_myExit();
            return;
        }
        printColor(ORANGE, "Cambiando la prioridad del proceso...\n");
        int resultado = usys_my_nice(nice_pid, newPrio);
        if (resultado == -1)
        {
            print("ups\n");
            printColor(RED, "No se pudo cambiar la prioridad del proceso.\n");
            usys_myExit();
            return;
        }
        else
        {
            printColor(GREEN, "Se cambio la prioridad del proceso: \n");
            intToStr(nice_pid, pid);
            print(pid);
            printColor(GREEN, " a la prioridad: ");
            intToStr(newPrio, prio);
            print(prio);
            printColor(GREEN, " !!! :) \n");
            putChar('\n');
            usys_myExit();
            return;
        }
    }
    usys_myExit();
}

void wc(char **params)
{
    int c, lineCount = 0;
    char buffer[3];
    putChar('\n');

    while ((c = getChar()) != EOF)
    {
        if (c == '\n')
        {
            lineCount++;
            putChar('\n');
        }
        else if (c >= 32 && c <= 126)
        {
            putChar(c);
        }
    }

    printColor(YELLOW, "\nNumber of lines: \n");
    intToStr(lineCount, buffer);
    print(buffer);
    putChar('\n');
    usys_myExit();
}

void cat(char **params)
{
    char buffer[PAGE] = {0}; // Buffer para almacenar caracteres
    char c;
    int idx = 0, pos = 0;

    while ((c = getChar()) != (char)EOF)
    {
        if (c == '\b')
        { // Manejo de backspace
            if (idx > 0)
            {
                putChar('\b');
                idx--;
            }
        }
        else if (c == '\n')
        {
            putChar('\n');
            for (pos = 0; pos < idx; pos++)
                putChar(buffer[pos]);

            idx = 0;
            putChar('\n');
        }
        else if (c >= 32 && c <= 126)
        {
            if (idx < PAGE)
            {
                buffer[idx++] = c;
                putChar(c);
            }
        }
    }
    putChar('\n');
    usys_myExit();
}

void filter(char **params)
{
    char buffer[PAGE] = {0};
    char c;
    int idx = 0;
    int pos = 0;
    while ((c = getChar()) != -1)
    {
        if (c > 20 && c < 127)
        {
            putChar(c);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'I' || c == 'E' ||
                c == 'O' || c == 'U') // Vocales
                buffer[idx++] = c;
        }
        if (c == '\n')
        {
            putChar('\n');
            putChar('\n');
            printColor(YELLOW, "Vocales: \n");
            while (pos < idx)
            {
                putChar(buffer[pos]);
                pos++;
            }
            idx = 0;
            pos = 0;
            putChar('\n');
        }
        if (c == '\b')
        {
            if (idx != 0)
            {
                pos--;
                putChar('\b');
            }
        }
    }
    usys_myExit();
}

void phylos()
{
    printColor(ORANGE, "Testeando sincronizacion con filosofos...\n");
    char *argv1[] = {"10", "1", "0"};
    phyloProcess(3, argv1);
    usys_myExit();
}