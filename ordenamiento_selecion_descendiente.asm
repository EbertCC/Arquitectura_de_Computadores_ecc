# Ordenaci�n por Selecci�n en Orden Descendente

.data
array: .word 5, 2, 9, 1, 5, 6   # Array inicial
n: .word 6                      # N�mero de elementos en el array
newline: .asciiz "\n"
space: .asciiz " "
msg_original: .asciiz "Array original: "
msg_sorted: .asciiz "Array ordenado en orden descendente: "

.text
.globl main
main:
    # Imprimir array original
    la $a0, msg_original
    li $v0, 4          # Syscall para imprimir cadena
    syscall

    la $t0, array      # $t0 apunta al inicio del array
    lw $t1, n          # Cargar el n�mero de elementos en $t1
    li $t2, 0          # �ndice inicial

print_original_loop:
    bge $t2, $t1, print_original_end
    lw $a0, 0($t0)     # Cargar elemento actual en $a0
    li $v0, 1          # Syscall para imprimir entero
    syscall

    addi $t2, $t2, 1   # Incrementar �ndice

    blt $t2, $t1, print_space_original  # Si no es el �ltimo elemento, imprimir espacio
    j advance_pointer_original

print_space_original:
    la $a0, space      # Imprimir espacio
    li $v0, 4
    syscall

advance_pointer_original:
    addi $t0, $t0, 4   # Avanzar al siguiente elemento
    j print_original_loop

print_original_end:
    la $a0, newline    # Imprimir nueva l�nea
    li $v0, 4
    syscall

    # Inicializaci�n de punteros para ordenaci�n descendente
    la $a0, array       # $a0 apunta al inicio del array
    lw $t0, n           # Cargar el n�mero de elementos en $t0
    subi $t0, $t0, 1    # �ndice del �ltimo elemento
    sll $t0, $t0, 2     # Multiplicar por 4 (tama�o de palabra)
    add $a1, $a0, $t0   # $a1 apunta al �ltimo elemento

sort:
    bge $a0, $a1, done      # Si el inicio supera o es igual al final, est� ordenado
    jal max                 # Llamar a la funci�n max
    lw $t0, 0($a0)          # Cargar el primer elemento en $t0
    sw $t0, 0($v0)          # Copiar el primer elemento en la posici�n del m�ximo
    sw $v1, 0($a0)          # Colocar el m�ximo al inicio
    addi $a0, $a0, 4        # Mover el puntero del inicio hacia adelante
    j sort                  # Repetir el proceso

done:
    # Imprimir array ordenado
    la $a0, msg_sorted
    li $v0, 4          # Syscall para imprimir cadena
    syscall

    la $t0, array      # $t0 apunta al inicio del array
    lw $t1, n          # Cargar el n�mero de elementos en $t1
    li $t2, 0          # �ndice inicial

print_sorted_loop:
    bge $t2, $t1, print_sorted_end
    lw $a0, 0($t0)     # Cargar elemento actual en $a0
    li $v0, 1          # Syscall para imprimir entero
    syscall

    addi $t2, $t2, 1   # Incrementar �ndice

    blt $t2, $t1, print_space_sorted  # Si no es el �ltimo elemento, imprimir espacio
    j advance_pointer_sorted

print_space_sorted:
    la $a0, space      # Imprimir espacio
    li $v0, 4
    syscall

advance_pointer_sorted:
    addi $t0, $t0, 4   # Avanzar al siguiente elemento
    j print_sorted_loop

print_sorted_end:
    la $a0, newline    # Imprimir nueva l�nea
    li $v0, 4
    syscall

    li $v0, 10              # Salir del programa
    syscall

# Funci�n max: encuentra el m�ximo entre $a0 y $a1
# Retorna en $v0 la direcci�n del m�ximo y en $v1 el valor m�ximo
max:
    move $t1, $a0           # $t1 es el puntero actual
    lw $v1, 0($t1)          # Inicializar $v1 con el primer valor
    move $v0, $t1           # Inicializar $v0 con la direcci�n del primer valor
max_loop:
    lw $t2, 0($t1)          # Cargar el valor actual
    bgt $t2, $v1, max_update # Si $t2 > $v1, actualizar m�ximo
    j max_next
max_update:
    move $v1, $t2           # Actualizar valor m�ximo
    move $v0, $t1           # Actualizar direcci�n del m�ximo
max_next:
    addi $t1, $t1, 4        # Avanzar al siguiente elemento
    bgt $t1, $a1, max_end   # Si hemos pasado el final, terminar
    j max_loop
max_end:
    jr $ra                  # Retornar
