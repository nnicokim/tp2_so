#include <shell.h>

#define INPUT_SIZE 100
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
    {"beep", beep, "Emite un beep"},
    {"tpr", test_processes, "Testea los procesos"},
    {"tmm", test_mm, "Testea el gestor de memoria"},
    {"tprio", test_prio, "Testea la prioridad de los procesos"},
    {"printp", print_processes, "Imprime los procesos activos"},
    {"cp", create_one_process, "Crea un proceso"},
    {"loop", loop_print, "Imprime el PID del proceso ejecutandose cada 2 segs"},
    {"kill", kill_process_pid, "Mata un proceso dado un PID"},
    {"block", block_process_pid, "Bloquea un proceso dado un PID"},
    {"incp", increase_prio_pid, "Cambia la prioridad de un proceso dado un PID y una prioridad"},
    {"decp", decrease_prio_pid, "Cambia la prioridad de un proceso dado un PID y una prioridad"},
    {"nice", nice_pid, "Cambia la prioridad de un proceso dado un PID y una prioridad"},
};

static Command commandsNohelp[] = {
    {"mem", print_memory, "Imprime la memoria"}};

#define sizeofArr(arr) (sizeof(arr) / sizeof(arr[0]))
#define COMMAND_COUNT sizeofArr(commands)

void parseCommand(char *str)
{
    char argument[] = {0};

    if (strcmp(str, "") == 0)
    {
        return;
    }

    int argC = parseCommandArg(str);

    if (argC > 1)
    {
        printColor(YELLOW, "No puede haber ni un espacio ni mas de 1 argumento. Verificar los comandos de nuevo.\n");
    }

    for (int i = 0; i < COMMAND_COUNT; i++)
    {
        if (strcmp(str, commands[i].name_id) == 0)
        {
            (*commands[i].func)(argument);
            return;
        }
        else if (strcmp(str, commandsNohelp[i].name_id) == 0)
        {
            (*commandsNohelp[i].func)(argument);
            return;
        }
    }
    printError("Error: comando no diponible. Ingrese \"help\" para ver los comandos disponibles.\n");
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
}

void divzero()
{
    int a = 1; // rax??
    int b = 0;
    if (a / b == 1)
    {
        printError("This is wrong...");
    }
}
void invopcode()
{
    _invalid_opcode_exception();
}

void time()
{
    printColor(GREEN, "ART (Argentine Time): UTC/GMT -3 horas\n");
    _get_time();
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
}
void clear_shell()
{
    usys_clear_screen();
}

void beep()
{
    printColor(GREEN, "BEEP!!\n");
    usys_beep(1000, 10);
}

void play_eliminator()
{
    gameActive = 1;
    clear_shell();
    eliminator();
    clear_shell();
    gameActive = 0;
}

void test_processes()
{
    printColor(ORANGE, "Testeando procesos...\n");
    char *argvAux[] = {"10"};
    usys_test_processes(1, argvAux);
}

void test_prio()
{
    printColor(ORANGE, "Testeando prioridades...\n");
    print("Testeando prioridades...\n");
    usys_test_prio();
}

void test_mm()
{
    printColor(ORANGE, "Testeando MM...\n");
    char *argvmm[] = {"1000"};
    usys_test_mm(1, argvmm);
}

void print_processes()
{
    printColor(ORANGE, "Imprimiendo procesos...\n");
    usys_print_processes();
}

void print_memory()
{
    usys_print_memory();
}

void create_one_process()
{
    printColor(ORANGE, "Creando un proceso...\n");
    usys_createProcess();
}

void loop_print()
{
    printColor(ORANGE, "Imprimiendo el PID de un proceso cada 2 segundos...\n");
    usys_loop_print();
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
                    inc_pid = stringToInt(pid);
                    break;
                }
                pid[i++] = c;
            }
        }

        if (inc_pid < 0 || inc_pid > MAX_PROCESS)
        {
            printColor(RED, "PID invalido. Ingrese un PID valido.\n");
            return;
        }

        printColor(ORANGE, "Aumentando la prioridad del proceso...\n");
        int resultado = usys_increase_priority(inc_pid);
        if (resultado == -1)
        {
            printColor(RED, "No se pudo incrementar la prioridad del proceso.\n");
            return;
        }
        else
        {
            printColor(GREEN, "Se incremento la prioridad del proceso :) \n");
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
                    dec_pid = stringToInt(pid);
                    break;
                }
                pid[i++] = c;
            }
        }

        if (dec_pid < 0 || dec_pid > MAX_PROCESS)
        {
            printColor(RED, "PID invalido. Ingrese un PID valido.\n");
            return;
        }

        printColor(ORANGE, "Decrementando la prioridad del proceso...\n");
        int resultado = usys_decrease_priority(dec_pid);
        if (resultado == -1)
        {
            printColor(RED, "No se pudo decrementar la prioridad del proceso.");
            print("\n");
            return;
        }
        else
        {
            printColor(GREEN, "Se decremento la prioridad del proceso :) \n");
            print("\n");
            return;
        }
    }
}

void nice_pid()
{
    print("Falta hacer, lo hago mañana\n");
}
