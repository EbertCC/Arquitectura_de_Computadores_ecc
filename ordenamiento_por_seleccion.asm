# Ordenaci�n por Selecci�n en MIPS

.data
array: .word 5, 2, 9, 1, 5, 6   # Array inicial
n: .word 6                      # N�mero de elementos en el array

.text
.globl main
main:
    # Inicializaci�n de punteros
    la $a0, array       # $a0 apunta al inicio del array
    lw $t0, n           # Cargar el n�mero de elementos en $t0
    subi $t0, $t0, 1    # �ndice del �ltimo elemento
    sll $t0, $t0, 2     # Multiplicar por 4 (tama�o de palabra)
    add $a1, $a0, $t0   # $a1 apunta al �ltimo elemento

sort:
    beq $a0, $a1, done      # Si solo queda un elemento, est� ordenado
    jal max                 # Llamar a la funci�n max
    lw $t0, 0($a1)          # Cargar el �ltimo elemento en $t0
    sw $t0, 0($v0)          # Copiar el �ltimo elemento en la posici�n del m�ximo
    sw $v1, 0($a1)          # Colocar el m�ximo al final
    addi $a1, $a1, -4       # Mover el puntero del final hacia atr�s
    j sort                  # Repetir el proceso

done:
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

