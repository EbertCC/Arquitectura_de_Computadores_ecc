#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cmath>

using namespace std;

//para 32 bist formato float
union float_32bits {
    float f;
    struct {
        uint32_t mantisa : 23;
        uint32_t exponente : 8;
        uint32_t signo : 1;
    } partes;
};

//imprimir 32 bits
void imrprimir_32bits(uint32_t valor, int numBits) {
    for (int i = numBits - 1; i >= 0; i--) {
        cout << ((valor >> i) & 1);
    }
}

//dividir 32 bits float
float_32bits floatDivision(float_32bits X, float_32bits Y) {
    float_32bits Z;
    bool X_isZero = (X.partes.exponente == 0 && X.partes.mantisa == 0);
    bool Y_isZero = (Y.partes.exponente == 0 && Y.partes.mantisa == 0);

    if (X_isZero && !Y_isZero) {
        Z.partes.signo = X.partes.signo ^ Y.partes.signo; 
        Z.partes.exponente = 0;
        Z.partes.mantisa = 0;
        return Z;
    }

    if (!X_isZero && Y_isZero) {
        Z.partes.signo = X.partes.signo ^ Y.partes.signo;
        Z.partes.exponente = 0xFF;//overflow
        Z.partes.mantisa = 0;
        return Z;
    }

    if (X_isZero && Y_isZero) {
        Z.partes.signo = 0;
        Z.partes.exponente = 0xFF;
        Z.partes.mantisa = 1;    
        return Z;
    }
    // Signo
    uint32_t signX = X.partes.signo;
    uint32_t signY = Y.partes.signo;
    uint32_t signZ = signX ^ signY; // XOR para dividir

    int32_t expX = X.partes.exponente;
    int32_t expY = Y.partes.exponente;

    uint32_t mantisaX = (expX == 0) ? X.partes.mantisa : (1 << 23) | X.partes.mantisa;
    uint32_t mantisaY = (expY == 0) ? Y.partes.mantisa : (1 << 23) | Y.partes.mantisa;
    int32_t realExpX = (expX == 0) ? 1 : expX;  
    int32_t realExpY = (expY == 0) ? 1 : expY; 
    int32_t expZ = (realExpX - realExpY) + 127; 

    //dividir mantisas
    uint64_t mX_64 = (uint64_t)mantisaX << 24; 
    uint64_t mY_64 = (uint64_t)mantisaY;
    if (mY_64 == 0) {
        Z.partes.signo = signZ;
        Z.partes.exponente = 0xFF;
        Z.partes.mantisa = 0;
        return Z;
    }

    uint64_t mZ_64 = mX_64 / mY_64; 
    int shift = 0;
    while (mZ_64 && (mZ_64 < (1ULL << 23))) {
        mZ_64 <<= 1;
        expZ--;
        shift++;
    }
    while (mZ_64 >= (1ULL << 24)) {
        mZ_64 >>= 1;
        expZ++;
        shift--;
    }
    //verificar overflow/underflow del exponente
    if (expZ >= 255) {
        Z.partes.signo = signZ;
        Z.partes.exponente = 0xFF;
        Z.partes.mantisa = 0;
        return Z;
    } else if (expZ <= 0) {
        Z.partes.signo = signZ;
        Z.partes.exponente = 0;
        Z.partes.mantisa = 0; 
        return Z;
    }
    //redondeo y resultado final
    uint32_t finalMantisa = (uint32_t)(mZ_64 & 0x7FFFFF); 
    Z.partes.signo = signZ;
    Z.partes.exponente = (uint32_t)expZ;
    Z.partes.mantisa = finalMantisa;
    return Z;
}

int main() {
    float_32bits X, Y, Z;
    cout <<"ingresar x : ";
    cin >> X.f;
    cout <<"ingresar y : ";
    cin >> Y.f;
    Z = floatDivision(X, Y);

    cout<<endl<<endl;
    cout<<"resultado de x/y : "<< Z.f<<" float ";
    cout<<endl;
    cout<<"signo: " << Z.partes.signo<< ", exponente: "<<dec<<(int)Z.partes.exponente<<", mantisa: "<<Z.partes.mantisa;
    cout<<endl;
    cout<<"binario: "; imrprimir_32bits(Z.partes.signo, 1); cout<<" ";imrprimir_32bits(Z.partes.exponente, 8);cout<<" ";imrprimir_32bits(Z.partes.mantisa, 23);
    cout<<endl;

    return 0;
}
