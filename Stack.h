/**
    \file
    File with functions for using my stack
    It has constructor, destructor, push, pop and resize.
    There is also defend from wrong using of it.

    Thank you for using this program!
    \warning Please, make your own type of stack by undefing stack_elem and elem\n
             Please, include math.h, stdlib.h and stdio.h to your file
    \authors Anna Savchuk
    \todo    Print out canaries and addresses
    \date    Last update was 08.10.20 at 13:34
*/

typedef enum stack_code_errors { STACK_OK,
                                 STACK_NULL,
                                 STACK_SEG_FAULT,
                                 STACK_DELETED,
                                 STACK_NO_CONSTRUCT,
                                 STACK_UNDERFLOW,
                                 STACK_NO_MEMORY,
                                 STACK_TOO_BIG,
                                 STACK_DEAD_CANARY,
                                 STACK_INVADERS    } stack_code;


const char *STACK_CONSTRUCT     = "CONSTRUCTOR";
const char *STACK_DESTRUCT      = "DESTRUCTOR";
const char *STACK_PUSH          = "PUSHING";
const char *STACK_RESIZE        = "RESIZING";
const char *STACK_POP           = "POPPING";

typedef double stack_elem;
#define elem "%lg"

#define ASSERTION(code)                                                   \
    fprintf(stderr, "-------------!WARNING!------------\n");              \
    fprintf(stderr, "IN FILE %s\nIN LINE %d\n", __FILE__, __LINE__);      \
    assertion(code);                                                      \

struct Stack_struct
{
    int         canary_before;
    stack_elem *buffer;
    int         canary_after;
    size_t      length;
    size_t      capacity;
    long int    hash_buffer;
    long int    hash_stack;
};

struct Defeat_stack
{
    int           canary_first;
    Stack_struct *stack;
    int           canary_last;
};

typedef struct Stack_struct Structure;
typedef struct Defeat_stack Stack;

static Stack *cage_copy;

/*!
Prints the errors in the console
@param[in]  code            The identifier of the error
*/
void              assertion           (stack_code code);

/*!
Outputs the information about the current state of the stack into the file
@param[in]  *log            The file to write in
@param[in]  *that_stack     The pointer on the shell of the stack
*/
void              print_state_stack   (FILE *log, Stack *that_stack);

/*!
Outputs the information about the current state of the stack into "log.txt"
@param[in]  *that_stack     The pointer on the shell of the stack
@param[in]  code            The code of the mistake
@param[in]  who             The code o the function requested for dump
*/
void              stack_dump          (Stack *that_stack, stack_code code, const char *who);

/*!
Checks if the pointer is valid
@param[in]  *that_stack     The pointer on the shell of the stack

Returns  STACK_NULL         If the pointer does not exists
         STACK_SEG_FAULT    If the pointer points in prohibited for using block of memory
         STACK_OK           If the pointer is valid
*/
stack_code        is_pointer_valid    (Stack *that_stack);

/*!
Counts hash for the buffer in the stack by algorythm Adler-32
@param[in]  *that_stack     The pointer on the shell of the stack

@param[out] hash_sum        The hash of the buffer in the stack
*/
long int          hashing_buffer      (Stack *that_stack);

/*!
Counts hash for the whole stack by algorythm Adler-32
@param[in]  *that_stack     The pointer on the shell of the stack

@param[out] hash_sum        The hash of the whole stack
*/
long int          hashing_stack       (Stack *that_stack);

/*!
Checks if the stack was spoiled and returns it to last saved correct version if it was
@param[in]  *that_stack     The pointer on the shell of the stack

Returns  STACK_CHANGED      If the stack was spoiled
         STACK_OK           If everything is ok
*/
static stack_code reserve_copy        (Stack *that_stack, Stack *copy_stack);


/*!
Checks all of the states of the stack
@param[in]  *that_stack     The pointer on the shell of the stack

Returns  STACK_CHANGED      If the stack was spoiled !!!!!!!!!!!!!!!!!!!!!!!!!!!!
         STACK_OK           If everything is ok
*/
stack_code        stack_verifier      (Stack *that_stack);

/*!
Creates new stack
@param[in]  size            The size of the future buffer of the stack

@param[out] *stack_tmp      The pointer on the shell of the future stack
*/
Stack            *stack_new           (size_t size);

/*!
Constructs the stack
@param[in]  *that_stack     The pointer on the shell of the future stack

Returns  STACK_OK           If everything is ok
*/
stack_code        stack_construct     (Stack *that_stack, size_t stack_size);

/*!
Destructs the stack
@param[in]  *that_stack     The pointer on the shell of the stack

Returns  STACK_OK           If everything is ok
*/
stack_code        stack_destruct      (Stack *that_stack);

/*!
Changes the capacity of the stack
@param[in]  *that_stack     The pointer on the shell of the stack

Returns  STACK_OK           If everything is ok
*/
stack_code        stack_resize        (Stack *that_stack, const double amount);

/*!
Adds value to the end of the stack
@param[in]  *that_stack     The pointer on the shell of the stack
@param[in]  value           The value wanted to be pushed

Returns  STACK_OK           If everything is ok
*/
stack_code        stack_push          (Stack *that_stack, stack_elem value);


/*!
Delets value from the end of the stack
@param[in]  *that_stack     The pointer on the shell of the stack
@param[in]  *value          The pointer on the value wanted to be pushed

Returns  STACK_OK           If everything is ok
*/
stack_code        stack_pop           (Stack *that_stack, stack_elem *value);

void assertion (stack_code code)
{
    switch (code)
    {
        case STACK_NULL:
            printf("Error: NO POINTER ON STACK FOUND\n");
            break;
        case STACK_SEG_FAULT:
            printf("Error: USAGE OF PROHIBITED PART OF MEMORY\n");
            break;
        case STACK_DELETED:
            printf("Error: DEALING WITH NON-EXISTENT UNIT OR THE UNIT WAS DAMAGED\n");
            break;
        case STACK_NO_CONSTRUCT:
            printf("Error: NO MEMORY FOR CONSTRUCTION\n");
            break;
        case STACK_UNDERFLOW:
            printf("Error: GOING OUT OF BONDS OF STACK\n");
            break;
        case STACK_NO_MEMORY:
            printf("Error: NO FREE MEMORY\n");
            break;
        case STACK_TOO_BIG:
            printf("Error: TOO BIG CAPACITY REQUIRED\n");
            break;
        case STACK_DEAD_CANARY:
            printf("Error: THE UNIT WAS DAMAGED\n");
            break;
        case STACK_INVADERS:
            printf("Error: OUTSIDE INVASION INSIDE OF STACK\n");
            break;
        default:
            break;
    }
}

void print_state_stack(FILE *log, Stack *that_stack)
{
    fprintf(log, "Current capacity: %u\n", that_stack->stack->capacity);
    fprintf(log, "Current size: %u\n", that_stack->stack->length);
    for (size_t i = 0; i < that_stack->stack->length; i++)
    {
        fprintf(log, "[%4u] : " elem "\n", i, that_stack->stack->buffer[i]);
    }
    for (size_t i = that_stack->stack->length; i < that_stack->stack->capacity; i++)
    {
        fprintf(log, "[%4u] : %s\n", i, "NAN (POISON)");
    }
}

void stack_dump (Stack *that_stack, stack_code code, const char *who)
{
    static long int doing = 0;

    char *mode =  "";
    if (!doing)
        mode = "wb";
    else
        mode = "ab";
    FILE *log = fopen("log.txt", mode);

    fprintf(log, "CURRENT STATE OF STACK\n");
    fprintf(log, "THE NEWS FROM %s\n", who);

    switch (code)
    {
        case STACK_OK:

            fprintf(log, "EVERYTHING IS OKAY\n");
            print_state_stack(log, that_stack);
            break;

        case STACK_NULL:

            fprintf(log, "STACK DOES NOT EXIST\n");
            break;

        case STACK_SEG_FAULT:

            fprintf(log, "MEMORY ACCESS DENIED\n");
            break;

        case STACK_DELETED:

            fprintf(log, "DEALING WITH NON-EXISTENT UNIT OR THE UNIT WAS DAMAGED\n");
            break;

        case STACK_NO_CONSTRUCT:

            fprintf(log, "NO MEMORY FOR CONSTRUCTION\n");
            break;

        case STACK_UNDERFLOW:

            fprintf(log, "NOTHING TO POP\n");
            print_state_stack(log, that_stack);
            break;

        case STACK_NO_MEMORY:

            fprintf(log, "MEMORY ACCESS DENIED\n");
            print_state_stack(log, that_stack);
            break;

        case STACK_TOO_BIG:

            fprintf(log, "TOO BIG CAPACITY IS REQUIRED\n");
            print_state_stack(log, that_stack);
            break;

        case STACK_DEAD_CANARY:

            fprintf(log, "THE UNIT WAS DAMAGED\n");
            print_state_stack(log, that_stack);
            break;

        case STACK_INVADERS:

            fprintf(log, "THE INVASION WAS DETECTED\n");
            print_state_stack(log, that_stack);
            break;

        default:
            break;
    }

    fprintf(log, "\n");

    fclose(log);

    doing++;
}

stack_code is_pointer_valid (Stack *that_stack)
{
    if (!that_stack || !that_stack->stack || !that_stack->stack->buffer)
        return STACK_NULL;

    if ((long long)(that_stack->stack) <= 4096 || (long long)(that_stack->stack->buffer) <= 4096)
        return STACK_SEG_FAULT;

    if (*(int *)that_stack != 0x5E7CA6E || *(int *)that_stack->stack != 0xDEADB14D ||
        !isnan(that_stack->stack->buffer[0]))
    {
        return STACK_DELETED;
    }

    return STACK_OK;
}

long int hashing_buffer (Stack *that_stack)
{
    long int hash_sum = 0;

    long int sum        = 1;
    long int prefix_sum = 0;

    for (char* i = (char*)(that_stack->stack->buffer); i < (char*)that_stack->stack->buffer +
                   (that_stack->stack->length) * sizeof(that_stack->stack->buffer[0]); i++)
    {
        sum = (sum + *i) % 65521;
        prefix_sum = (prefix_sum + sum) % 65521;
    }

    hash_sum = (prefix_sum << 16) + sum;

    return hash_sum;
}

long int hashing_stack (Stack *that_stack)
{
    long int hash_tmp             = that_stack->stack->hash_stack;
    that_stack->stack->hash_stack = 0;

    long int hash_sum = 0;

    long int sum        = 1;
    long int prefix_sum = 0;

    for (char* i = (char*)(that_stack->stack); i < (char*)(that_stack->stack) + sizeof((that_stack->stack)); i++)
    {
        sum = (sum + *i) % 65521;
        prefix_sum = (prefix_sum + sum) % 65521;
    }

    hash_sum = (prefix_sum << 16) + sum;

    that_stack->stack->hash_stack = hash_tmp;
    return hash_sum;
}

static stack_code reserve_copy (Stack *that_stack, Stack *copy_stack)
{
    that_stack->stack->hash_stack  = copy_stack->stack->hash_stack;
    that_stack->stack->hash_buffer = copy_stack->stack->hash_buffer;
    that_stack->stack->length      = copy_stack->stack->length;
    that_stack->stack->capacity    = copy_stack->stack->capacity;

    for (size_t i = 0; i < copy_stack->stack->length; i++)
    {
        that_stack->stack->buffer[i] = copy_stack->stack->buffer[i];
    }
    that_stack->stack->buffer[that_stack->stack->length + 1] = NAN;

    return STACK_OK;
}

stack_code stack_verifier (Stack *that_stack)
{
    stack_code indicator = is_pointer_valid(that_stack);
    if (indicator != STACK_OK)
    {
        return indicator;
    }
    indicator = is_pointer_valid(cage_copy);
    if (indicator != STACK_OK)
    {
        return indicator;
    }

    long int hash_tmp_stack     = hashing_stack(that_stack);
    long int hash_tmp_stack_buf = hashing_buffer(that_stack);

    long int hash_tmp_copy      = hashing_stack(cage_copy);
    long int hash_tmp_copy_buf  = hashing_buffer(cage_copy);

    long int flag_hash_stack      = (hash_tmp_stack == that_stack->stack->hash_stack);
    long int flag_hash_copy       = (hash_tmp_copy  == cage_copy->stack->hash_stack);
    long int flag_hash_stack_buf  = (hash_tmp_stack_buf == that_stack->stack->hash_buffer);
    long int flag_hash_copy_buf   = (hash_tmp_copy_buf  == cage_copy->stack->hash_buffer);

    if (that_stack->canary_first == 0x5E7CA6E && that_stack->canary_last == 0x0FFCA6E)
    {
        if (!flag_hash_stack && !flag_hash_copy && !flag_hash_stack_buf && !flag_hash_copy_buf)
        {
            return STACK_INVADERS;
        }
        else if (!flag_hash_stack && !flag_hash_copy && !flag_hash_stack_buf && flag_hash_copy_buf)
        {
            long int i       = 1;
            long int new_len = 0;

            while(!isnan(cage_copy->stack->buffer[i]))
            {
                new_len++;
                i++;
            }

            cage_copy->stack->length     = new_len;
            cage_copy->stack->capacity   = new_len + 1;
            cage_copy->stack->hash_stack = hash_tmp_copy;

            reserve_copy(that_stack, cage_copy);

            return STACK_OK;
        }
        else if (!flag_hash_stack && !flag_hash_copy && flag_hash_stack_buf && !flag_hash_copy_buf)
        {
            long int i       = 1;
            long int new_len = 0;

            while(!isnan(that_stack->stack->buffer[i]))
            {
                new_len++;
                i++;
            }

            that_stack->stack->length     = new_len;
            that_stack->stack->capacity   = new_len + 1;
            that_stack->stack->hash_stack = hash_tmp_stack;

            reserve_copy(cage_copy, that_stack);
        }
        else if ((!flag_hash_stack && flag_hash_copy && !flag_hash_stack_buf && !flag_hash_copy_buf) ||
                (flag_hash_stack && !flag_hash_copy && !flag_hash_stack_buf && !flag_hash_copy_buf))
        {
            return STACK_INVADERS;
        }
    }
    else
    {
        return STACK_DEAD_CANARY;
    }

    return STACK_OK;
}

Stack *stack_new(size_t size)
{
    Stack *cage         = (Stack*) calloc(1, sizeof(Stack));
    cage->canary_first  = 0x5E7CA6E;
    cage->stack         = (Structure*) calloc(1, sizeof(Structure));
    cage->canary_last   = 0x0FFCA6E;

    if (stack_construct(cage, size) == STACK_NO_CONSTRUCT)
    {
        stack_destruct(cage);
    }

    return cage;
}

stack_code stack_construct(Stack *that_stack, size_t stack_size)
{
    that_stack->stack->canary_before = 0xDEADB14D;
    that_stack->stack->capacity      = stack_size + 2;
    that_stack->stack->buffer        = (stack_elem*) calloc(stack_size + 2, sizeof(stack_elem));
    if (!that_stack->stack->buffer)
    {
        ASSERTION(STACK_NO_CONSTRUCT);
        stack_dump(that_stack, STACK_NO_MEMORY, STACK_CONSTRUCT);
        return STACK_NO_CONSTRUCT;

    }
    that_stack->stack->buffer[0]     = NAN;
    that_stack->stack->length       = 1;
    that_stack->stack->hash_buffer  = hashing_buffer(that_stack);
    that_stack->stack->hash_stack   = hashing_stack (that_stack);

    cage_copy               = (Stack*) calloc(1, sizeof(Stack));
    cage_copy->canary_first = 0x5E7CA6E;
    cage_copy->stack        = (Structure*) calloc(1, sizeof(Structure));
    cage_copy->canary_last  = 0x0FFCA6E;

    cage_copy->stack->canary_before = 0xDEADB14D;
    cage_copy->stack->capacity      = stack_size + 2;
    cage_copy->stack->buffer        = (stack_elem*) calloc(stack_size + 2, sizeof(stack_elem)); //!!!!!!!!!!
    if (!cage_copy->stack->buffer)
    {
        ASSERTION(STACK_NO_CONSTRUCT);
        stack_dump(that_stack, STACK_NO_MEMORY, STACK_CONSTRUCT);
        return STACK_NO_CONSTRUCT;
    }
    cage_copy->stack->buffer[0]     = NAN;
    cage_copy->stack->length       = 1;
    cage_copy->stack->hash_buffer  = hashing_buffer(cage_copy);
    cage_copy->stack->hash_stack   = hashing_stack (cage_copy);

    return STACK_OK;
}

stack_code stack_destruct(Stack *that_stack)
{
    stack_code check = stack_verifier(that_stack);
    if (check != STACK_OK)
    {
        ASSERTION(check);
        stack_dump(that_stack, check, STACK_DESTRUCT);
        return check;
    }
    else
        stack_dump(that_stack, STACK_OK, STACK_DESTRUCT);

    if (that_stack)
    {
        if (that_stack->stack)
        {
            if (that_stack->stack->buffer)
                free(that_stack->stack->buffer);
            free(that_stack->stack);
        }
        free(that_stack);
    }

    if (cage_copy)
    {
        if (cage_copy->stack)
        {
            if (cage_copy->stack->buffer)
                free(cage_copy->stack->buffer);
            free(cage_copy->stack);
        }
        free(cage_copy);
    }

    return STACK_OK;
}

stack_code stack_resize(Stack *that_stack, const double amount)
{
    stack_code check = stack_verifier(that_stack);
    if (check != STACK_OK)
    {
        ASSERTION(check);
        stack_dump(that_stack, check, STACK_RESIZE);
    }

    if (that_stack->stack->capacity > ((size_t)-1)/2)
    {
        ASSERTION(STACK_TOO_BIG);
        stack_dump(that_stack, STACK_TOO_BIG, STACK_RESIZE);
    }

    size_t new_capacity = (size_t)(that_stack->stack->capacity * amount) + 2;

    stack_elem *ptr = (stack_elem*)realloc(that_stack->stack->buffer, sizeof(stack_elem) * new_capacity);

    if (!ptr)
    {
        ASSERTION(STACK_NO_MEMORY);
        stack_dump(that_stack, STACK_NO_MEMORY, STACK_RESIZE);
    }

    that_stack->stack->capacity = new_capacity;
    that_stack->stack->buffer   = ptr;

    return STACK_OK;
}

stack_code stack_push(Stack *that_stack, const stack_elem value)
{
    stack_code check = stack_verifier(that_stack);
    if (check != STACK_OK)
    {
        ASSERTION(check);
        stack_dump(that_stack, check, STACK_PUSH);
        return check;
    }

    if (that_stack->stack->length + 1 >= that_stack->stack->capacity)
    {
        stack_resize(that_stack, 2);
        stack_resize(cage_copy,  2);
    }

    that_stack->stack->buffer[that_stack->stack->length++] = value;
    that_stack->stack->buffer[that_stack->stack->length]   = NAN;

    that_stack->stack->hash_buffer = hashing_buffer(that_stack);
    that_stack->stack->hash_stack  = hashing_stack (that_stack);

    cage_copy->stack->buffer[cage_copy->stack->length++] = value;
    cage_copy->stack->buffer[cage_copy->stack->length]   = NAN;

    cage_copy->stack->hash_buffer = hashing_buffer(cage_copy);
    cage_copy->stack->hash_stack  = hashing_stack (cage_copy);

    return STACK_OK;
}

stack_code stack_pop(Stack *that_stack, stack_elem *value)
{
    stack_code check = stack_verifier(that_stack);
    if (check != STACK_OK)
    {
        ASSERTION(check);
        stack_dump(that_stack, check, STACK_POP);
    }
    if (that_stack->stack->length <= 0)
    {
        ASSERTION(STACK_UNDERFLOW);
        stack_dump(that_stack, STACK_UNDERFLOW, STACK_POP);
        return STACK_UNDERFLOW;
    }

    if (that_stack->stack->length <= that_stack->stack->capacity/2)
    {
        stack_resize(that_stack, 0.5);
        stack_resize(cage_copy,  0.5);
    }

    *value = that_stack->stack->buffer[--that_stack->stack->length];
    that_stack->stack->buffer[that_stack->stack->length] = NAN;

    that_stack->stack->hash_buffer = hashing_buffer(that_stack);
    that_stack->stack->hash_stack  = hashing_stack(that_stack);

    cage_copy->stack->buffer[cage_copy->stack->length--] = NAN;

    cage_copy->stack->hash_buffer = hashing_buffer(cage_copy);
    cage_copy->stack->hash_stack  = hashing_stack(cage_copy);

    return STACK_OK;
}