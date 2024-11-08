#include <shell.h>
#include "./include/tests/test_processes.h"
#include "./include/tests/test_prio.h"
#include "./include/tests/test_mm.h"

#define INPUT_SIZE 100
// #define COMMAND_COUNT 10
#define CANT_REGS 18

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
void call_test_processes();
void call_test_mm();
void call_test_prio();
void print_processes();
void print_memory();

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
    // SO
    {"tpr", call_test_processes, "Testea los procesos"},
    {"tmm", call_test_mm, "Testea el gestor de memoria"},
    {"tprio", call_test_prio, "Testea la prioridad de los procesos"},
    {"printp", print_processes, "Imprime los procesos activos"}};

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
    printColor(PINK, "Comandos disponibles:\n");
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

void call_test_processes()
{
    printColor(ORANGE, "Prrocesses testing...\n");
    char *argvAux[] = {"10"};
    test_processes(1, argvAux);
}

void call_test_prio()
{
    printColor(ORANGE, "Priorities testing...\n");
    test_prio();
}

void call_test_mm()
{
    printColor(ORANGE, "MM testing...\n");
    char *argvmm[] = {"1000"};
    test_mm(1, argvmm);
}

void print_processes()
{
    printColor(ORANGE, "Printing processes...\n");
    usys_print_processes();
}

void print_memory()
{
    printColor(ORANGE, "Printing memory directions...\n");
    usys_print_memory();
}
