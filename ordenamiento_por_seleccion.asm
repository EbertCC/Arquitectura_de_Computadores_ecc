# Ordenación por Selección en MIPS

.data
array: .word 5, 2, 9, 1, 5, 6   # Array inicial
n: .word 6                      # Número de elementos en el array

.text
.globl main
main:
    # Inicialización de punteros
    la $a0, array       # $a0 apunta al inicio del array
    lw $t0, n           # Cargar el número de elementos en $t0
    subi $t0, $t0, 1    # Índice del último elemento
    sll $t0, $t0, 2     # Multiplicar por 4 (tamaño de palabra)
    add $a1, $a0, $t0   # $a1 apunta al último elemento

sort:
    beq $a0, $a1, done      # Si solo queda un elemento, está ordenado
    jal max                 # Llamar a la función max
    lw $t0, 0($a1)          # Cargar el último elemento en $t0
    sw $t0, 0($v0)          # Copiar el último elemento en la posición del máximo
    sw $v1, 0($a1)          # Colocar el máximo al final
    addi $a1, $a1, -4       # Mover el puntero del final hacia atrás
    j sort                  # Repetir el proceso

done:
    li $v0, 10              # Salir del programa
    syscall

# Función max: encuentra el máximo entre $a0 y $a1
# Retorna en $v0 la dirección del máximo y en $v1 el valor máximo
max:
    move $t1, $a0           # $t1 es el puntero actual
    lw $v1, 0($t1)          # Inicializar $v1 con el primer valor
    move $v0, $t1           # Inicializar $v0 con la dirección del primer valor
max_loop:
    lw $t2, 0($t1)          # Cargar el valor actual
    bgt $t2, $v1, max_update # Si $t2 > $v1, actualizar máximo
    j max_next
max_update:
    move $v1, $t2           # Actualizar valor máximo
    move $v0, $t1           # Actualizar dirección del máximo
max_next:
    addi $t1, $t1, 4        # Avanzar al siguiente elemento
    bgt $t1, $a1, max_end   # Si hemos pasado el final, terminar
    j max_loop
max_end:
    jr $ra                  # Retornar

