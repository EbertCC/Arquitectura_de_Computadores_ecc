#include <iostream>
#include <bitset>
#include <cmath>
#include <cstdint>
using namespace std;

//funcion para convertir a complemento a 2 mi numero
//toTwosComplement
long long Complemento_dos(long long x, int n){
    long long copi = (1LL << n) - 1;  
    x &= copi;                       
    long long bit_signo = 1LL << (n - 1);
    if (x & bit_signo){
        // extension de signo
        long long copi_2 = ~((1LL << n) - 1);
        x |= copi_2;
    }
    return x;
}


//imprimir en binario
void imprimir_binario(long long x, int n){
    const int MAX_BITS = 64;
    bitset<MAX_BITS> b(x);
    for (int i = n - 1; i >= 0; i--){
        cout<< b[i];
    }
}

// suma/resta bits(n) complemento 2
long long sum_rest_nbist(long long a, long long b, int n){
    long long copi = (1LL << n) - 1;
    long long resultado = (a + b) & copi;
    return Complemento_dos(resultado, n);
}

// ---------------------------------------------------------------------------
// Función principal: implementa el algoritmo de Booth.
int main() {
    ios::sync_with_stdio(false);
    int n;
    long long multiplicando, multiplicador;

    cout<<"ingrese numero bist<32 ";
    cin>>n;

    cout<<"ingrese multiplicando : ";
    cin>> multiplicando;
    cout<<"ingrese multiplicador : ";
    cin>> multiplicador;

    //valores iniciales
    long long A   = 0;                              
    long long Q   = Complemento_dos(multiplicando, n);  
    long long M   = Complemento_dos(multiplicador, n);  
    long long Q_1 = 0;                          
    cout<<"\n--- valores iniciales ---\n";
    cout<<"A   = "; imprimir_binario(A, n); cout << "  (decimal " << A << ")"<<endl;
    cout<<"Q   = "; imprimir_binario(Q, n); cout << "  (decimal " << Q << ")"<<endl;
    cout<<"Q_1 = " << Q_1 << "\n";
    cout<<"M   = "; imprimir_binario(M, n); cout << "  (decimal " << M << ")"<<endl;
    cout<<endl;
    //contador i<n veces
    for(int i = 0; i < n; i++) {
        long long Q0 = (Q & 1LL);  // LSB
        // evaluando (Q0, Q_1)
        if (Q0 == 0 && Q_1 == 1) {
            A = sum_rest_nbist(A, M, n);
            cout<< "[iteracion " << i+1 << "] (Q0,Q_1)=(0,1) => A = A + M => ";
            imprimir_binario(A, n);
            cout<< " (decimal " << A << ")"<<endl;
        }
        else if (Q0 == 1 && Q_1 == 0) {
            // -M en complemento 2
            long long m_negado = sum_rest_nbist(~M, 1, n);
            A = sum_rest_nbist(A, m_negado, n);
            cout<< "[iteracion " << i+1 << "] (Q0,Q_1)=(1,0) => A = A - M => ";
            imprimir_binario(A, n);
            cout<< " (decimal " << A << ")"<<endl;
        }
        else {
            cout<< "[iteracion " << i+1 << "] (Q0,Q_1)=(" << Q0 << "," << Q_1 << ") => no se hizo suma/resta\n";
        }
        //desplazamiento a la derecha
        long long A_Q_Q1 = ((A & ((1LL << n) - 1)) << (n + 1)) | ((Q & ((1LL << n) - 1)) << 1) | (Q_1 & 1LL);
        // MSB de A
        long long signo_A =  (A >> (n - 1)) & 1LL;
        A_Q_Q1 >>= 1;
        // MSB A:1, se completa a la izquierda.
        if (signo_A == 1) {
            long long copi_A = ((1LL << n) - 1) << n;
            A_Q_Q1 |= copi_A;
        }
        //actualizacion de Q_1, Q y A 
        Q_1 = A_Q_Q1 & 1LL;
        Q   = (A_Q_Q1 >> 1) & ((1LL << n) - 1);
        A   = (A_Q_Q1 >> (n + 1)) & ((1LL << n) - 1);
        A   = Complemento_dos(A, n);
        cout<<"  Shift => A="; imprimir_binario(A, n);
        cout<<"  Q="; imprimir_binario(Q, n);
        cout<<"  Q_1=" << Q_1;
        cout<<"  (decimal A= " << A << ", decimalQ= " << Q << ")";
        cout<<endl<<endl;
    }

    //resultado
    long long A_Q_final = ((A & ((1LL << n) - 1)) << n) | (Q & ((1LL << n) - 1));
    long long resultado_nbist = Complemento_dos(A_Q_final, 2*n);

    cout << "----resultado ----"<<endl;
    cout << "bist (" << 2*n << "): ";
    imprimir_binario(resultado_nbist, 2*n);
    cout<<endl;
    cout <<"Valor decimal (complemento a 2 de " << 2*n << " bits): "<< resultado_nbist <<endl<<endl;
    // Verificación directa:
    long long verificacion = multiplicando * multiplicador;
    cout << "(verificacion decimales : " <<multiplicando<<" * "<<multiplicador<<" = "<< verificacion<<")"<<endl;
    return 0;
}
