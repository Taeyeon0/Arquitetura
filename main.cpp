#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <stdint.h>
 
/*Poxim1 rodando perfeitamente*/
/*SOFTWARE INTERRUPTION funcionando*/
/*TERMINAL funcionando*/
 
using namespace std;
 
void start_operation(uint32_t &IR, vector<uint32_t> memory, uint32_t PC,
                     uint32_t &OP, uint32_t &y, uint32_t &x, uint32_t &z, uint32_t &IM16, uint32_t &IM26){
    IR = memory[PC]; //Armazenando IR
    OP = ( IR & 0xFC000000 ) >> 26; // Cálculo da Operação
    y = (IR & 0x0000001F); // Operando y
    x = (IR & 0x000003E0) >> 5; // Operando x
    z = (IR & 0x00007C00) >> 10;  // Operando z
    IM16 = (IR & 0x03FFFC00) >> 10; //Imediato 16
    IM26 = IR & 0x03FFFFFF; //Imediato 26
 
    cout << hex << IR << endl;
    cout << hex << OP << endl;
}
 
void finish_operation(ofstream &saida, uint32_t &PC, uint32_t &CR, uint32_t IM26){
    PC = IM26; //Ou seja, zero (0).
    CR = 0;
    saida<<dec<<"int "<<IM26<<endl;
    saida<<"[S] ";
    saida<<"CR = 0x00000000, ";
    saida<<hex<<"PC = 0x"<<setw(8)<<setfill('0')<<uppercase<<PC<<endl;
};
 
void substitute_U(uint32_t z, int n, ofstream &saida){
    switch (z){
        case 32: //PC
            if (n == 1){
                saida << "pc";
            }
            else{
                saida << "PC";
            }
            break;
        case 33: //IR
            if (n == 1){
                saida << "ir";
            }
            else{
                saida << "IR";
            }
            break;
        case 34: //ER
            if (n == 1){
                saida << "er";
            }
            else{
                saida << "ER";
            }
            break;
        case 35: //FR
            if (n == 1){
                saida << "fr";
            }
            else{
                saida << "FR";
            }
            break;
        default:
            if (n == 1) {
                saida << "r" << dec << z;
            } else{
                saida << "R" << dec << z;
            }
 
    }
}
 
 
void print_something(ofstream &saida, uint32_t OP, uint32_t IM26, uint32_t IM16, uint32_t x,
                    uint32_t y, uint32_t z, uint32_t PC, uint32_t ER, uint32_t FR,uint32_t R[]){
    switch(OP){
        case 0x0:
            saida << "add "; substitute_U(z,1,saida); saida << ", "; substitute_U(x,1,saida); saida << ", "; substitute_U(y,1,saida); saida << endl;
            saida << "[U] FR = " << hex << "0x" << setw(8) << setfill('0') << uppercase << FR;
            saida << ", "; substitute_U(z,0,saida); saida << " = "; substitute_U(x,0,saida); saida << " + "; substitute_U(y,0,saida); saida << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[z] << endl;
 
            /*saida<< "add r"<<dec<<z<<", r"<<dec<<x<<", r"<<dec<< y <<endl;
            saida << "[U] ";
            saida<<"FR = 0x"<<hex<<setw(8)<<setfill('0')<<uppercase<<FR<<", ";
            saida<<dec<<"R"<<z<<" = R"<<dec<<x<<" + R"<<dec<<y<<" = 0x"<<hex<<setw(8)<<setfill('0')<<uppercase<<R[z]<<endl;
            */
             break;
        case 0x1: //addi
            saida << "addi r" << dec << x << ", r" << dec << y << ", " << dec << IM16 << endl;
            saida << "[F] FR = " << hex << "0x" << setw(8) << setfill('0') << uppercase << FR << ", R" << dec << x << " = R" << dec << y << " + " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << " = "<< hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0x2: //sub
            saida << "sub "; substitute_U(z,1,saida); saida << ", "; substitute_U(x,1,saida); saida << ", "; substitute_U(y,1,saida); saida << endl;
            saida << "[U] FR = " << hex << "0x" << setw(8) << setfill('0') << uppercase << FR;
            saida << ", "; substitute_U(z,0,saida); saida << " = "; substitute_U(x,0,saida); saida << " - "; substitute_U(y,0,saida); saida << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[z] << endl;
            break;
        case 0x3: //subi
            saida << "subi r" << dec << x << ", r" << dec << y << ", " << dec << IM16 << endl;
            saida << "[F] FR = " << hex << "0x" << setw(8) << setfill('0') << uppercase << FR << ", R" << dec << x << " = R" << dec << y << " - " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0x4: //mul
            saida << "mul "; substitute_U(z,1,saida); saida << ", "; substitute_U(x,1,saida); saida << ", "; substitute_U(y,1,saida); saida << endl;
            saida << "[U] FR = " << hex << "0x" << setw(8) << setfill('0') << uppercase << FR << ", ER = " << hex << "0x" << setw(8) << setfill('0') << ER;
            saida << ", "; substitute_U(z,0,saida); saida << " = "; substitute_U(x,0,saida); saida << " * "; substitute_U(y,0,saida); saida << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[z] << endl;
            break;
        case 0x5: //muli
            saida << "muli r" << dec << x <<", r" << dec << y << ", " << dec << IM16 << endl;
            saida << "[F] FR = " << hex << "0x" << setw(8) << setfill('0') << uppercase << FR << ", ER = " << hex << "0x" << setw(8) << setfill('0') << ER << ", R" << dec << x << " = R" << dec << y << " * " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0x6: //div
            saida << "div "; substitute_U(z,1,saida); saida << ", "; substitute_U(x,1,saida); saida << ", "; substitute_U(y,1,saida); saida << endl;
            saida << "[U] FR = " << hex << "0x" << setw(8) << setfill('0') << uppercase << FR << ", ER = " << hex << "0x" << setw(8) << setfill('0') << ER;
            saida << ", "; substitute_U(z,0,saida); saida << " = "; substitute_U(x,0,saida); saida << " / "; substitute_U(y,0,saida); saida << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[z] << endl;
            break;
        case 0x7:
            saida << "divi r" << dec << x << ", r" << dec << y << ", " << dec << IM16 << endl;
            saida << "[F] FR = " << hex << "0x" << setw(8) << setfill('0')
                  << uppercase << FR << ", ER = " << hex << "0x" << setw(8) << setfill('0')
                  << uppercase << ER << ", R" << dec << x << " = R" << dec << y << " / " << hex << "0x"
                  << setw(4) << setfill('0') << uppercase << IM16 << " = " << hex << "0x" << setw(8)
                  << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0x8:
            saida << "cmp "; substitute_U(x,1,saida); saida << ", "; substitute_U(y,1,saida); saida << endl;
            saida<<"[U] ";
            saida<<hex<<"FR = 0x"<<setw(8)<<setfill('0')<<uppercase<<FR<<endl;
            break;
        case 0x9:
            saida << "cmpi r" << dec << x << ", " << dec << IM16 << endl;
            saida<<"[F] ";
            saida<<hex<<"FR = 0x"<<setw(8)<<setfill('0')<<uppercase<<FR<<endl;
            break;
        case 0xA:
            saida << "shl "; substitute_U(z,1,saida); saida << ", "; substitute_U(x,1,saida); saida << ", " << dec << y << endl;
            saida << "[U] ER = " << hex << "0x" << setw(8) << setfill('0') << uppercase << ER;
            saida << ", "; substitute_U(z,0,saida); saida << " = "; substitute_U(x,0,saida); saida << " << " << dec << (y + 1) << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[z] << endl;
            break;
        case 0x0B:
            saida << "shr "; substitute_U(z,1,saida); saida << ", "; substitute_U(x,1,saida); saida << ", " << dec << y << endl;
            saida << "[U] ER = " << hex << "0x" << setw(8) << setfill('0') << uppercase << ER;
            saida << ", "; substitute_U(z,0,saida); saida << " = "; substitute_U(x,0,saida); saida << " >> " << dec << (y + 1) << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[z] << endl;
            break;
        case 0xC: //and
            saida << "and "; substitute_U(z,1,saida); saida << ", "; substitute_U(x,1,saida); saida << ", "; substitute_U(y,1,saida); saida << endl;
            saida << "[U] "; substitute_U(z,0,saida); saida << " = "; substitute_U(x,0,saida); saida << " & "; substitute_U(y,0,saida); saida << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[z] << endl;
            break;
        case 0xD: //andi
            saida << "andi r" << dec << x << ", r" << dec << y << ", " << dec << IM16 << endl;
            saida << "[F] R" << dec << x << " = R" << dec << y << " & " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0xE: //not
            saida << "not "; substitute_U(x,1,saida); saida << ", "; substitute_U(y,1,saida); saida << endl;
            saida << "[U] "; substitute_U(x,0,saida); saida << " = ~"; substitute_U(y,0,saida); saida << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0xF: //noti
            saida << "noti r" << dec << x << ", " << dec << IM16 << endl;
            saida << "[F] R" << dec << x << " = ~" << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0x10: //or
            saida << "or "; substitute_U(z,1,saida); saida << ", "; substitute_U(x,1,saida); saida << ", "; substitute_U(y,1,saida); saida << endl;
            saida << "[U] "; substitute_U(z,0,saida); saida << " = "; substitute_U(x,0,saida); saida << " | "; substitute_U(y,0,saida); saida << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[z] << endl;
            break;
        case 0x11: //ori
            saida << "ori r" << dec << x << ", r" << dec << y << ", " << dec << IM16 << endl;
            saida << "[F] R" << dec << x << " = R" << dec << y << " | " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0x12: //xor
            saida << "xor "; substitute_U(z,1,saida); saida << ", "; substitute_U(x,1,saida); saida << ", "; substitute_U(y,1,saida); saida << endl;
            saida << "[U] "; substitute_U(z,0,saida); saida << " = "; substitute_U(x,0,saida); saida << " ^ "; substitute_U(y,0,saida); saida << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[z] << endl;
            break;
        case 0x13: //xori
            saida << "xori r" << dec << x << ", r" << dec << y << ", " << dec << IM16 << endl;
            saida << "[F] R" << dec << x << " = R" << dec << y << " ^ " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0x1A: // bun
            saida<<hex<<"bun 0x"<<setw(8)<<setfill('0')<<uppercase<<IM26<< endl;
            saida<<hex<<"[S] PC = 0x"<<setw(8)<<setfill('0')<<uppercase<< (PC << 2) << endl;
            break;
        case 0x14: //ldw
            saida << "ldw r" << dec << x << ", r" << dec << y << ", " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << endl;
            saida << "[F] R" << dec << x << " = MEM[(R" << dec << y << " + " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << ") << 2] = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0x15: //ldb
            saida << "ldb r" << dec << x << ", r" << dec << y << ", " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << endl;
            saida << "[F] R" << dec << x << " = MEM[R" << dec << y << " + " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << "] = " << hex << "0x" << setw(2) << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0x16: //stw
            saida << "stw r" << dec << x << ", " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << ", r"<< dec << y << endl;
            saida << "[F] MEM[(R" << dec << x << " + " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << ") << 2] = R" << dec << y << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[y] << endl;
            break;
        case 0x17:
            saida << "stb r"<< dec << x << ", " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << ", r"<< dec << y << endl;
            saida << "[F] MEM[R" << dec << x << " + " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << "] = R"<< dec << y << " = " << hex << "0x" << setw(2) << setfill('0') << uppercase <<  (R[y] & 0x000000FF) << endl;
            break;
        case 0x18: //push
            saida << "push "; substitute_U(x,1,saida); saida <<  ", "; substitute_U(y,1,saida); saida << endl;
            saida << "[U] MEM["; substitute_U(x,0,saida); saida << "--] = "; substitute_U(y,0,saida); saida << " = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[y] << endl;
            break;
        case 0x19: //pop
            saida << "pop "; substitute_U(x,1,saida); saida << ", "; substitute_U(y,1,saida); saida << endl;
            saida << "[U] "; substitute_U(x,0,saida); saida << " = MEM[++"; substitute_U(y,0,saida); saida << "] = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << endl;
            break;
        case 0x1E: //bne
            saida << "bne " << hex << "0x" << setw(8) << setfill('0') << uppercase << IM26 << endl;
            saida << "[S] PC = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
            break;
        case 0x25: //call
            saida << "call r" << dec << x << ", r" << dec << y << ", " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << endl;
            saida << "[F] R" << dec << x << " = (PC + 4) >> 2 = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << ", PC = (R" << dec << y << " + " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << ") << 2 = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
            break;
        case 0x26: //ret
            saida << "ret r" << dec << x << endl;
            saida << "[F] PC = R" << dec << x << " << 2 = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
            break;
        case 0x27: //isr
            saida << "isr "; substitute_U(x,1,saida); saida << ", "; substitute_U(y,1,saida); saida << ", " << hex << "0x" << setw(4) << setfill('0') << uppercase << IM16 << endl;
            saida << "[F] "; substitute_U(x,0,saida); saida << " = IPC >> 2 = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[x] << ", "; substitute_U(y,0,saida); saida << " = CR = " << hex << "0x" << setw(8) << setfill('0') << uppercase << R[y] << ", PC = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
            break;
        default:
            saida << "[INVALID INSTRUCTION @ " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << "]" << endl;
            saida << "[SOFTWARE INTERRUPTION]" << endl;
            break;
    }
}
 
void change_operators(uint32_t  IR, uint32_t &x, uint32_t &y, uint32_t &z){
    if ((IR & 0x10000) == 0x10000)
        x = (x & (~0x00000020)) | 0x20;
    if ((IR & 0x8000) == 0x8000)
        y = (y & (~0x00000020)) | 0x20;
    if ((IR & 0x20000) == 0x20000)
        z = (z & (~0x00000020)) | 0x20;
        cout << "O valor de z eh -->" << dec << z << endl;
}
 
 
void bun_operation (uint32_t &PC, uint32_t IM26){
    PC = IM26;
    //IM26 = (IM26) << 2;
}
 
void add_operation (uint32_t &z, uint32_t x , uint32_t y, uint32_t &FR){
    if ( ( ((uint64_t)(x) + (uint64_t)(y))) > 0xFFFFFFFF){
        FR = (FR & 0xFFFFFFEF);
        FR = FR + 0x00000010;
    } else{
        FR = FR & 0x0000006F;
    }
    z = x + y;
    cout << "O valor de x --->" << dec << x << endl;
    cout << "O valor de y --->" << dec << y << endl;
    cout << "O valor da adicao foi--->" << dec << z << endl;
}
 
void div_operation(uint32_t &z, uint32_t x , uint32_t y,uint32_t &FR, uint32_t &ER){
    if (y != 0x00000000)
    {
        ER = x % y;
        z = x / y;
        FR = (FR & (~0x00000018)) | 0x00;
    }
    else
        FR = (FR & (~0x8)) | 0x8;
 
    /*if (y == 0){
        ER = 0;
        FR = ((FR & 0xFFFFFFF7)+ (01 << 3));
    } else{
        ER = x % y;
        z = x/y;
    }*/
}
 
void cmp_operation(uint32_t x, uint32_t y, uint32_t &FR){
    if(x == y){
        FR = (FR & 0xFFFFFFF8);
        FR = FR + 1;
    }
    else if(x < y){
        FR = ((FR & 0xFFFFFFF8) + (01<<1));
    }
    else{
        FR = ((FR & 0xFFFFFFF8) + (01<<2));
    }
}
 
void shl_operation(uint32_t  x, uint32_t y, uint32_t &z, uint32_t &ER){
    uint64_t aux1 = ER;
    aux1 = (aux1 << 32) | x;
    uint64_t aux2 = aux1 << (y + 1);
    ER = (aux2 & 0xFFFFFFFF00000000) >> 32;
    z = (aux2 & 0x00000000FFFFFFFF);
}
 
void shr_operation(uint32_t  x, uint32_t y, uint32_t &z, uint32_t &ER){
    uint64_t aux1 = ER;
    aux1 = (aux1 << 32) | x;
    uint64_t aux2 = aux1 >> (y + 1);
    ER = (aux2 & 0xFFFFFFFF00000000) >> 32;
    z = (aux2 & 0x00000000FFFFFFFF);
}
 
void ldb_operation(vector<uint32_t> memory, uint32_t &x, uint32_t y, uint32_t IM16,vector<char> terminal){
    switch ((y + IM16) >> 2) {
        case 0x2222:
            x = terminal.back();  //dando problema
            break;
        default:
            uint32_t aux = (y + IM16)%4;
            switch (aux) {
                case 3:
                    x = memory[(y + IM16) / 4];
                    x = (x & 0x000000FF);
                    break;
                case 2:
                    x = (memory[((y + IM16) >> 2)] & 0x0000FF00) >> 8;
                    break;
                case 1:
                    x = (memory[((y + IM16) >> 2)] & 0x00FF0000) >> 16;
                    break;
                default:
                    x = (memory[((y + IM16) >> 2)] & 0xFF000000) >> 24;
                    break;
            }
    }
}
 
void stb_operation(vector<uint32_t> &memory, uint32_t &x, uint32_t &y, uint32_t &IM16,
                    vector<char>&terminal){
 
    switch ((x + IM16) >> 2){
        case 0x2222:
            terminal.push_back(y & 0xFF);
            break;
        default:
            switch((x + IM16) - (((x + IM16)>>2)<<2)) {
                case 0x0000:
                    memory[(x + IM16) >> 2] = (memory[(x + IM16) >> 2] & (~0xFF000000)) | (y << 24);
                    break;
                case 0x0001:
                    memory[(x + IM16) >> 2] = (memory[(x + IM16) >> 2] & (~0x00FF0000)) | (y << 16);
                    break;
                case 0x0002:
                    memory[(x + IM16) >> 2] = (memory[(x + IM16) >> 2] & (~0x0000FF00)) | (y << 8);
                    break;
                case 0x0003:
                    memory[(x + IM16) >> 2] = (memory[(x + IM16) >> 2] & (~0x000000FF)) | y;
                    break;
            }
            break;
    }
}
 
void mul_operation(uint32_t x, uint32_t y, uint32_t &z, uint32_t &ER, uint32_t &FR){
    uint64_t aux1;
    aux1 = (uint64_t) x * y;
    ER = (aux1 & 0xFFFFFFFF00000000) >> 32;
    z = (aux1 & 0x00000000FFFFFFFF);
    if (ER != 0){
        FR = (FR & 0x0000007F);
        FR =  (FR | 0x00000010);
    }
    else{
        FR = FR & 0x0000006F;
    }
}
 
void push_operation(vector<uint32_t>&memory, uint32_t &x, uint32_t y){
    memory[x + 0x0000] = y;
    x = x - 1;
}
 
void pop_operation(vector<uint32_t>memory, uint32_t &x, uint32_t &y){
    y = y + 1;
    x = memory[y];
}
 
void bne_operation(uint32_t FR, uint32_t &PC, uint32_t IM26) {
    if (((FR & 0x00000001) >> 0) == 1) {
        PC = PC + 1;
    } else {
        PC = (IM26);
    }
}
 
void call_operation(uint32_t &x, uint32_t y, uint32_t &PC, uint32_t IM16, uint32_t R[]){
    x = (PC + 1);
    R[0] = 0;
    PC = (y + IM16);
}
 
void ret_operation(uint32_t &PC, uint32_t x){
    PC = x;
}
 
void sub_operation(uint32_t &z, uint32_t x, uint32_t y, uint32_t &FR) {
    if (((uint64_t) x - (uint64_t) y) > 0xFFFFFFFF) {
        FR = (FR & (~0x00000010)) | 0x10;
        z = x - y;
    } else {
        FR = (FR & (~0x00000010)) | 0x00;
        z = x - y;
    }
}
 
void bge_operation(uint32_t FR, uint32_t &PC, uint32_t IM26){
    if( ( ( FR & 0x00000001 ) >> 0 ) == 1  || ( ( FR & 0x00000004 ) >> 2 ) == 1 ) {
        PC = ((IM26));
    } else
        PC = PC + 1;
}
 
void ble_operation(uint32_t FR, uint32_t &PC, uint32_t IM26){
    if( ( ( FR & 0x00000001 ) >> 0 ) == 1  || ( (FR & 0x00000002 ) >> 1 ) == 1 ) {
        PC = (IM26);
    } else
        PC = PC + 1;
}
 
void bgt_operation(uint32_t FR, uint32_t &PC, uint32_t IM26){
    if(((FR & 0x00000004) >> 2 ) == 1 ) {
        PC = (IM26);
    } else
        PC = PC + 1;
}
 
void blt_operation(uint32_t FR, uint32_t &PC, uint32_t IM26){
    if ( ( (FR & 0x00000002 ) >> 1 ) == 1 ) {
        PC = (IM26);
    } else
        PC = PC + 1;
}
 
void beq_operation(uint32_t FR, uint32_t &PC, uint32_t IM26){
    if ((FR & 0x00000001) == 0x1)
        PC = IM26;
    else
        PC = PC + 1;
}
 
void and_operation(uint32_t &z, uint32_t x, uint32_t y){
    z = x & y;
}
 
void or_operation(uint32_t &z, uint32_t x, uint32_t y){
    z = x | y;
}
 
void xor_operation(uint32_t &z, uint32_t x, uint32_t y){
    z = x ^ y;
}
 
void not_operation(uint32_t &x, uint32_t y){
    x = ~y;
}
 
void bzd_operation(uint32_t FR, uint32_t &PC, uint32_t IM26){
    if ((FR & 0x8) == 0x8) {
        PC = IM26;
    }
    else PC = PC + 1;
}
 
void biv_operation(uint32_t FR, uint32_t &PC, uint32_t IM26){
    if ((FR & 0x20) == 0x20) {
        PC = IM26;
    }
    else PC = PC + 1;
}
 
void isr_operation(uint32_t &x, uint32_t IPC, uint32_t &y, uint32_t CR, uint32_t &PC, uint32_t IM16){
    x = IPC;
    y = CR;
    PC = IM16;
}
 
void stw_operation(uint32_t x, uint32_t y,uint32_t IM16, vector<char>&terminal, vector<uint32_t> &memory) {
    uint32_t aux_l = (x + IM16);
    switch (aux_l) {
        case 0x2222:
            terminal.push_back(y & 0xFF);
            break;
        default:
            memory[x + IM16] = y;
            break;
    }
}
 
void ldw_operation(uint32_t &x, uint32_t y,uint32_t IM16, vector<char>terminal, vector<uint32_t> memory){
    uint32_t aux_l = y + IM16;
    switch (aux_l){
        case 0x2222: //lendo para o terminal
            x = terminal[aux_l];
            break;
        default:
            x = memory[y + IM16];
            break;
    }
}
 
 
/*Operações de Interrupção*/
 
void software_interruption(uint32_t OP, ofstream &saida, uint32_t FR, uint32_t IM16, uint32_t IM26,
                           uint32_t &CR, uint32_t &PC, uint32_t &IPC){
    switch (OP){
        case 0x6: //div
            if ((FR & 0x40) == 0x40) {
                saida << "[SOFTWARE INTERRUPTION]" << endl;
                CR = 1;
                IPC = PC;
                PC = 3;
            }
            break;
        case 0x7:
            if (((FR & 0x40) == 0x40) && (IM16 == 0)) {
                saida << "[SOFTWARE INTERRUPTION]" << endl;
                CR = 1;
                IPC = PC;
                PC = 3;
            }
            break;
        default:
            break;
 
    }
}
 
void print_terminal(vector<char>terminal, ofstream &saida){
    if(!terminal.empty()) {
        saida << "[TERMINAL]" << endl;
        for (vector<char>::iterator it = terminal.begin() ; it != terminal.end(); ++it)
            saida << *it;
        saida << endl;
    }
}
 
/*Operações de Interrupção*/
 
 
//using namespace std;
 
int main (int argc, char *argv[]){
    ifstream entrada(argv[1]);
    ofstream saida(argv[2]);
 
    /*Defines para melhorar a estrutura do programa*/
    uint32_t  R[38] = {0x0};
    #define PC  R[32]
    #define IR  R[33]
    #define ER  R[34]
    #define FR  R[35]
    #define CR  R[36]
    #define IPC R[37]
 
    /*Defines para melhorar a estrutura do programa*/
 
 
    /*Variaveis necessárias*/
 
    vector<uint32_t> memory; //Vector memória
    vector<char> terminal; // Vector do Terminal
    uint32_t in_memory; //Variável para preencher valores hex na memória
    uint32_t  OP = 0;  //Operação
    uint32_t IM26 = 0; //Imediato 26
    uint32_t IM16 = 0; // Imediato 16
    uint32_t x, y, z; //Operandos
    bool termino = true; //Controle do while
    uint32_t aux_l, aux_w;
 
    /*Variaveis necessárias*/
 
 
    while(entrada >> hex >> in_memory ){
        memory.push_back(in_memory);
    }
 
    saida <<"[START OF SIMULATION]" << endl;
 
    while(termino){
        R[0] = 0x0;
        start_operation(IR,memory,PC,OP,y,x,z,IM16,IM26);
 
        switch (OP){
            case 0x0: //add
                change_operators(IR,x,y,z);
                add_operation(R[z],R[x],R[y],FR);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x1: //addi
                add_operation(R[x],R[y],IM16,FR);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1; //Incrementando operação
                break;
            case 0x2: //sub
                change_operators(IR,x,y,z);
                sub_operation(R[z],R[x],R[y],FR);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x3: //subi
                sub_operation(R[x],R[y],IM16,FR);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x4: //mul
                change_operators(IR,x,y,z);
                mul_operation(R[x],R[y],R[z],ER,FR);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x5: //muli /*Operação dando errada*/
                mul_operation(R[y],IM16,R[x],ER,FR);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x6: //div
                change_operators(IR,x,y,z);
                div_operation(R[z],R[x],R[y],FR,ER);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                software_interruption(OP,saida,FR,IM16,IM26,CR,PC,IPC);
                break;
            case 0x7: //divi
                div_operation(R[x],R[y],IM16,FR,ER);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1; //Incrementando operação
                software_interruption(OP,saida,FR,IM16,IM26,CR,PC,IPC);
                break;
            case 0x8: //cmp
                change_operators(IR, x, y ,z);
                cmp_operation(R[x], R[y], FR);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x9: //cmpi
                cmp_operation(R[x], IM16, FR);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x0A: //shl
                change_operators(IR, x, y ,z);
                shl_operation(R[x], y, R[z], ER);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0xB: //shr
                change_operators(IR, x, y ,z);
                shr_operation(R[x], y, R[z], ER);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0xC: //and
                change_operators(IR, x, y ,z);
                and_operation(R[z], R[x], R[y]);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0xD: //andi
                and_operation(R[x],R[y],IM16);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0xE: //not
                change_operators(IR, x, y ,z);
                not_operation(R[x],R[y]);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0xF: //noti
                not_operation(R[x],IM16);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x10: //or
                change_operators(IR, x, y ,z);
                or_operation(R[z], R[x], R[y]);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x11: //ori
                or_operation(R[x], R[y], IM16);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x12: //xor
                change_operators(IR, x, y ,z);
                xor_operation(R[z], R[x], R[y]);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x13: //xori
                xor_operation(R[x], R[y], IM16);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x1A: //bun
                bun_operation(PC, IM26);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                break;
            case 0x1B: //beq
                saida << "beq " << hex << "0x" << setw(8) << setfill('0') << uppercase << IM26 << endl;
                beq_operation(FR,PC,IM26);
                saida << "[S] PC = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
                break;
            case 0x1C: //blt
                saida << "blt " << hex << "0x" << setw(8) << setfill('0') << uppercase << IM26 << endl;
                blt_operation(FR,PC,IM26);
                saida << "[S] PC = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
                break;
            case 0x1D: //bgt
                saida << "bgt " << hex << "0x" << setw(8) << setfill('0') << uppercase << IM26 << endl;
                bgt_operation(FR,PC,IM26);
                saida << "[S] PC = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
                break;
            case 0x1F: //ble
                saida << "ble " << hex << "0x" << setw(8) << setfill('0') << uppercase << IM26 << endl;
                ble_operation(FR,PC,IM26);
                saida << "[S] PC = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
                break;
            case 0x14: //ldw
                ldw_operation(R[x],R[y],IM16,terminal,memory);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1; //Incrementando operação
                break;
            case 0x16: //stw FALTA O RESTO DA OPERAÇÃO
                stw_operation(R[x],R[y],IM16,terminal,memory);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x15: //ldb
                ldb_operation(memory,R[x],R[y],IM16,terminal);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x17: //stb
                x = (R[33] & 0X000003E0)>>5;
                y = (R[33] & 0x0000001f);
                IM16 = (R[33] & 0x03fffc00)>>10;
                stb_operation(memory,R[x],R[y],IM16,terminal);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x18: //PUSH
                change_operators(IR,x,y,z);
                push_operation(memory,R[x], R[y]);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x19: //POP
                change_operators(IR,x,y,z);
                pop_operation(memory,R[x],R[y]);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                PC = PC + 1;
                break;
            case 0x20: //bge
                saida << "bge " << hex << "0x" << setw(8) << setfill('0') << uppercase << IM26 << endl;
                bge_operation(FR,PC,IM26);
                saida << "[S] PC = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
                break;
            case 0x21: //bzd
                bzd_operation(FR,PC,IM26);
                saida << "bzd " << hex << "0x" << setw(8) << setfill('0') << uppercase << IM26 << endl;
                saida << "[S] PC = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
                break;
            case 0x23: //biv
                biv_operation(FR,PC,IM26);
                saida << "biv " << hex << "0x" << setw(8) << setfill('0') << uppercase << IM26 << endl;
                saida << "[S] PC = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
                break;
            case 0x1E: // bne
                bne_operation(FR,PC,IM26);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                break;
            case 0x25: // call
                //R[0] = 0;
                call_operation(R[x],R[y],PC,IM16,R);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                break;
            case 0x26: //ret
                ret_operation(PC,R[x]);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                break;
            case 0x27: //isr
                isr_operation(R[x],IPC,R[y],CR,PC,IM16);
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                break;
            case 0x3f: //int (Finalizar Programa aqui)
                if (IM26 == 0){
                    finish_operation(saida,PC,CR,IM26);
                    print_terminal(terminal,saida);
                    saida<<"[END OF SIMULATION]"<<endl;
                    termino = false;
                } else{
                    /*Tratar o SOFTWARE INTERRUPTION AQUI*/
                    saida<<dec<<"int "<<IM26<<endl;
                    CR = IM26;
                    IPC = PC + 1;
                    PC = 3;
                    saida << "[S] CR = " << hex << "0x" << setw(8) << setfill('0') << uppercase << CR << ", PC = " << hex << "0x" << setw(8) << setfill('0') << uppercase << (PC << 2) << endl;
                    saida << "[SOFTWARE INTERRUPTION]" << endl;
                }
                break;
            default:
                /*Tratar o SOFTWARE INTERRUPTION AQUI*/
                print_something(saida,OP,IM26,IM16,x,y,z,PC,ER,FR,R);
                FR = (FR | 0x00000020);
                CR = PC;
                PC = PC + 1;
                IPC = PC;
                PC = 3;
                break;
        }
 
    }
 
    //saida << "Escrevendo" << endl; //escrevendo na saída EASY EASY
 
    return 0;
}
