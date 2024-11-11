#include <shell.h>

#define INPUT_SIZE 50
// #define COMMAND_COUNT 10
#define CANT_REGS 18
#define MAX_PROCESS 192
#define TRUE 1

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
void test_processes();
void test_mm();
void test_prio();
void print_processes();
void print_memory();
void create_one_process();
void loop_print();
void kill_process_pid();
void block_process_pid();
void increase_prio_pid();
void decrease_prio_pid();
void nice_pid();
void test_sync1();
void test_sync2();

static char buffer[INPUT_SIZE] = {0};
static int bufferIndex = 0;
static int currentFontSize;
static int gameActive = 0;

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
    {"beep", beep, "Emite un usys_beep"},
    {"victory", victory, "VAMOS CARAJO!!!"},
    {"mario", mario_bros_song, "Canta el himno de Mario Bros"},
    {"tpr", test_processes, "Testea los procesos"},
    {"tmm", test_mm, "Testea el gestor de memoria"},
    {"tprio", test_prio, "Testea la prioridad de los procesos"},
    {"printp", print_processes, "Imprime los procesos activos"},
    {"loop", loop_print, "Imprime el PID del proceso ejecutandose cada 2 segs"},
    {"kill", kill_process_pid, "Mata un proceso dado un PID"},
    {"block", block_process_pid, "Bloquea un proceso dado un PID"},
    {"incp", increase_prio_pid, "Cambia la prioridad de un proceso dado un PID y una prioridad"},
    {"decp", decrease_prio_pid, "Cambia la prioridad de un proceso dado un PID y una prioridad"},
    {"nice", nice_pid, "Cambia la prioridad de un proceso dado un PID y una prioridad"},
};

static Command commandsNohelp[] = {
    {"cp", create_one_process, "Crea un proceso"},
    {"mem", print_memory, "Imprime la memoria"},
    {"tsync1", test_sync1, "Testea la sincronizacion con semaforos"},
    {"tsync2", test_sync2, "Testea la sincronizacion sin semaforos"},
    {"egg", easteregg, "Easter egg song"},
};

#define sizeofArr(arr) (sizeof(arr) / sizeof(arr[0]))
#define COMMAND_COUNT sizeofArr(commands)


void parseCommand(char *str) // TODO: dependiendo de si es BG o FG, se envia el FD correspondiente
{
    int fgFD[] = { 0, 1 };
    int bgFD[] = { -1, -1 };
    
    char argument[] = {0};
    if (strcmp(str, "") == 0)
    {
        return;
    }

    int argC = parseCommandArg(str);
    int cmdLen = strlen(str);
    char lastChar = str[cmdLen-2]; 

    if (lastChar == '&')
        return handleCommands(str, bgFD);
    else
        return handleCommands(str, fgFD);
}

void handleRegularCommand(char * str, int * fd){
    char *argument[] = { 0 };

    if(strcmp(str, "") == 0) return;
    print("HandleRegularCommand\n");

    // TODO: parseCommandArg { char argc, char * argv }
    // char argc = parseCommandArg(str, argv);

    int argC = parseCommandArg(str);
    while(*str == ' ') str++;

    for (int i = 0; i < COMMAND_COUNT; i++)
    {
        if (strcmp(str, commands[i].name_id) == 0)
        {
            usys_createProcess(str, commands[i].func, argC, argument, fd);
            return;
        } else if (strcmp(str, commandsNohelp[i].name_id) == 0) {
            usys_createProcess(str, commandsNohelp[i].func, argC, argument, fd);
            return;
        }
    }
    printError("Error: comando no diponible. Ingrese \"help\" para ver los comandos disponibles.\n");
}

void handleCommands(char * str, int * fd){
    char * cmds[3] = { str, 0 };
    char * currCMD = str;
    uint64_t currentID = 0;
    while(*currCMD != '\n' && currentID < 3){
        if(*currCMD == '|'){
            *currCMD = '\0';
            cmds[currentID] = currCMD + 1;   
            char * aux = currCMD;

            currentID++;
        }

        currCMD++;
    }

    if(currentID >= 3) {
        printError("Demasiados comandos en la linea de comandos.\n");
        return;
    };
    for(int i = 0; i <= currentID; i++)
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
    int running = 1;
    currentFontSize = usys_get_font_size();
    printPromptIcon();
    while (running)
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
                // printPromptIcon();
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
            //else if ( c == ) {
             //   
            //}
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
            return;
        }
        else if (c == 'S' || c == 's')
        {
            if (currentFontSize >= 3)
            {
                printColor(RED, "No se puede agrandar mas.\n");
                return;
            }
            usys_change_font_size(++currentFontSize);
            clear_shell();
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
            return;
        }
        else if (c == 'S' || c == 's')
        {
            if (currentFontSize <= 1)
            {
                printColor(RED, "No se puede achicar mas.\n");
                return;
            }
            usys_change_font_size(--currentFontSize);
            clear_shell();
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
            print("\n");
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
    printColor(GREEN, "Mario Bros\n");

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
    usys_wait(150);
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

void test_processes()
{
    printColor(ORANGE, "Testeando procesos...\n");
    char *argvAux[] = {"10"};
    usys_test_processes(1, argvAux);
    usys_myExit();
}

void test_prio()
{
    printColor(ORANGE, "Testeando prioridades...\n");
    print("Testeando prioridades...\n");
    usys_test_prio();
    usys_myExit();
}

void test_mm()
{
    printColor(ORANGE, "Testeando MM...\n");
    char *argvmm[] = {"500"};
    usys_test_mm(1, argvmm);
    usys_myExit();
}

void print_processes()
{
    printColor(ORANGE, "Imprimiendo procesos...\n");
    usys_print_processes();
    printPromptIcon();
    usys_myExit();
}

void print_memory()
{
    printColor(ORANGE, "Imprimiendo memoria...\n");
    usys_print_memory();
    printPromptIcon();
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

void test_sync1()
{
    printColor(ORANGE, "Testeando sincronizacion con sincro...\n");
    char *argv1[] = {"10", "1", "0"}; // Para usar el de sync y el argc=3
    usys_test_sync(3, argv1);
    usys_myExit();
}

void test_sync2()
{
    printColor(ORANGE, "Testeando sincronizacion sin sincro...\n");
    char *argv2[] = {"10", "1", "1"}; // Para usar el de no sync y el argc=3
    usys_test_sync(3, argv2);
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
                    print("\n");
                    printColor(RED, "ERROR. Ingrese un digito valido.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    return;
                }
                if (i > 3)
                {
                    print("\n");
                    printColor(RED, "ERROR. PID muy largo.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
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
            return;
        }
        if (kill_pid == 0 || kill_pid == 1)
        {
            print("\n");
            printColor(RED, "No se puede matar el proceso SHELL o IDLE. Ingrese otro PID.\n");
            return;
        }

        printColor(ORANGE, "Matando al proceso...\n");
        int resultado = usys_killProcess(kill_pid);
        if (resultado == -1)
        {
            printColor(RED, "No se pudo matar al proceso con PID: ");
            intToStr(kill_pid, pid);
            print(pid);
            print("\n");
            return;
        }
        else
        {
            printColor(GREEN, "Se mato al proceso con PID: ");
            intToStr(kill_pid, pid);
            print(pid);
            print("\n");
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
                    print("\n");
                    printColor(RED, "ERROR. Ingrese un digito valido.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    return;
                }
                if (i > 3)
                {
                    print("\n");
                    printColor(RED, "ERROR. PID muy largo.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
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
            return;
        }
        if (block_pid == 0 || block_pid == 1)
        {
            printColor(RED, "No se puede bloquear el proceso SHELL o IDLE. Ingrese otro PID.\n");
            return;
        }

        printColor(ORANGE, "Bloqueando al proceso...\n");
        int resultado = usys_blockProcess(block_pid);
        if (resultado == -1)
        {
            printColor(RED, "No se pudo bloquear al proceso con PID: ");
            intToStr(block_pid, pid);
            print(pid);
            print("\n");
            return;
        }
        else
        {
            printColor(GREEN, "Se bloqueo al proceso con PID: ");
            intToStr(block_pid, pid);
            print(pid);
            print("\n");
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
                    print("\n");
                    printColor(RED, "ERROR. Ingrese un digito valido.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (i > 3)
                {
                    print("\n");
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
                    print("\n");
                    printColor(RED, "ERROR. Ingrese un digito valido.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    usys_myExit();
                    return;
                }
                if (i > 3)
                {
                    print("\n");
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
            print("\n");
            usys_myExit();
            return;
        }
        else
        {
            printColor(GREEN, "Se decremento la prioridad del proceso :) \n");
            print("\n");
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
                    print("\n");
                    printColor(RED, "ERROR. Ingrese un digito valido.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
                    return;
                }
                if (i > 3)
                {
                    print("\n");
                    printColor(RED, "ERROR. PID muy largo.\n");
                    printColor(YELLOW, "Vuelva a intentarlo.\n");
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
                        print("\n");
                        printColor(RED, "ERROR. Ingrese un espacio entre el PID y la prioridad.\n");
                        printColor(YELLOW, "Vuelva a intentarlo.\n");
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
            return;
        }
        if (newPrio < 0 || newPrio > 5)
        {
            printColor(RED, "Prioridad invalida. Ingrese una prioridad valida.\n");
            return;
        }
        printColor(ORANGE, "Cambiando la prioridad del proceso...\n");
        int resultado = usys_my_nice(nice_pid, newPrio);
        if (resultado == -1)
        {
            print("ups\n");
            printColor(RED, "No se pudo cambiar la prioridad del proceso.\n");
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
            print("\n");
            return;
        }
    }
    usys_myExit();
}
