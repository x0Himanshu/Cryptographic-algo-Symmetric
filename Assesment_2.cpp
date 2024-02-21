#include <iostream>
#include <bitset>
#include <math.h>
#include <algorithm>
using namespace std;


class SDES
{
    private:
    string s0[4][4]={{"01","00","11","10"},{"11","10","01","00"},{"00","10","01","11"},{"11","01","11","10"}};
    string s1[4][4]={{"00","01","10","11"},{"10","00","01","11"},{"11","00","01","00"},{"10","01","00","11"}};
    string subKey1,subKey2;

    protected:
    string IP(string str)
    {
        return string({str[1],str[5],str[2],str[0],str[3],str[7],str[4],str[6]});
    }
    string IP_inv(string str)
    {
        return string({str[3],str[0],str[2],str[4],str[6],str[1],str[7],str[5]});
    }
    string P_10(string str)
    {
        return string({str[2],str[4],str[1],str[6],str[3],str[9],str[0],str[8],str[7],str[5]});
    }
    string P_8(string str)
    {
        return string({str[5],str[2],str[6],str[3],str[7],str[4],str[9],str[8]});
    }
    string E_P(string R)
    {
        char out[8] = {R[3],R[0],R[1],R[2], R[1],R[2],R[3],R[0]};
        return out;

    }
    string P_4(string str)
    {
        return string({str[1],str[3],str[2],str[0]});
    }
    string XOR(string str1, string str2, int numBit)
    {
        string out;
        for(int i=0; i<numBit; i++)
        {
            if(str1[i]==str2[i]) out += '0';
            else out += '1';
        }
        return out;
    }
    void LCS(string &str, int num)
    {
        while(num>0)
        {   
            char temp = str[0];
            for(int i=0; i<5; i++)
            {
                str[i] = str[i+1];
            }
            str[4]=temp;
            num--;
        }
    }
    string Fiesatal(string L, string R,string subkey)
    {
        string out = E_P(R);
        cout<<"After Expansion and Permutation: "<<out<<endl;
        out = XOR(out,subkey,8);
        cout<<"After XOR: "<<out<<endl;

        string sbox0,sbox1;
        string temp;
        temp={out[0],out[3]};
        int row = stoi(temp,nullptr,2);
        cout<<"S-box-0 row="<<temp;
        temp = {out[1],out[2]};
        cout<<" col="<<temp<<endl;
        int col = stoi(temp,nullptr,2);
        sbox0 = s0[row][col];;

        temp = {out[4],out[7]};
        cout<<"S-box-1 row="<<temp;
        row = stoi(temp,nullptr,2);
        temp = {out[5],out[6]};
        cout<<" col="<<temp<<endl;
        col = stoi(temp,nullptr,2);
        sbox1 = s1[row][col];
        out = sbox0+sbox1;
        out = P_4(out);
        cout<<"After sbox P4:"<< out<<endl;
        
        out = XOR(out,L,4);
        return out; 
    }

    public:
    
    void KeyGeneration(string key)
    {
        key = P_10(key);
        cout<<"Key after P10: "<<key<<endl;
        
        string Left,Right;
        Left = key.substr(0,5);
        Right = key.substr(5,5);
        LCS(Left,1); LCS(Right,1);
        cout<<"Key after Left Circular shift by 1 bit: "<<Left<<" "<<Right<<endl;
        subKey1 = P_8(Left+Right);
        cout<<"Sub-Key-1 after P8: "<<subKey1<<endl;
        LCS(Left,2); LCS(Right,2);
        cout<<"Key after Left Circular shift by 2 bit: "<<Left<<" "<<Right<<endl;
        subKey2 = P_8(Left+Right);
        cout<<"Sub-Key-2 after P8: "<<subKey2<<endl<<endl;
    }
    string Encrypt(string plaintext)
    {
        cout<<"\n  Encryption\nPlaintext: "<<plaintext<<endl;
        plaintext = IP(plaintext);
        cout<<"Plain text after Initial Permutation: "<<plaintext<<endl;
        string Left,Right;
        Left = plaintext.substr(0,4);
        Right = plaintext.substr(4,4);
        cout<<"Values in Left and Right registers: "<<Left<<" "<<Right<<endl;
        cout<<"\nFiestal function "<<endl;
        string f1 = Fiesatal(Left,Right,subKey1);
        cout<<"Output of fyster function Round 1: "<<f1<<endl;
        Left = Right;
        Right = f1;
        cout<<"Values in Left and Right registers after swapping: "<<Left<<" "<<Right<<endl;
        cout<<"\nFiestal function "<<endl;
        f1 = Fiesatal(Left,Right,subKey2);
        cout<<"Output of fyster function Round 2: "<<f1<<endl;
        string cipher_text = f1.append(Right);
        cout<<"Final data in registers: "<<cipher_text<<endl;
        cipher_text = IP_inv(cipher_text);
        cout<<"After Inverse Initial Permutation: "<<cipher_text<<endl;
        return cipher_text;
    }
    string Decrypt(string cipher_text)
    {
        cout<<"\n  Decryption\nCipher Text: "<<cipher_text<<endl;
        cipher_text = IP(cipher_text);
        cout<<"Plain text after Initial Permutation: "<<cipher_text<<endl;
        string Left,Right;
        Left = cipher_text.substr(0,4);
        Right = cipher_text.substr(4,4);
        cout<<"Values in Left and Right registers: "<<Left<<" "<<Right<<endl;
        cout<<"\nFiestal function "<<endl;
        string f1 = Fiesatal(Left,Right,subKey2);
        cout<<"Output of fyster function Round 1: "<<f1<<endl;
        Left = Right;
        Right = f1;
        cout<<"Values in Left and Right registers after swapping: "<<Left<<" "<<Right<<endl;
        cout<<"\nFiestal function "<<endl;
        f1 = Fiesatal(Left,Right,subKey1);
        cout<<"Output of fyster function Round 2: "<<f1<<endl;
        string text = f1.append(Right);
        cout<<"Final data in registers: "<<text<<endl;
        text = IP_inv(text);
        cout<<"After Inverse Initial Permutation: "<<text<<endl;
        return text;
    }
    
};
class AES
{
    unsigned int RCon[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36};
    unsigned int s[16][16] =
    {
        {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
        {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
        {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
        {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
        {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
        {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
        {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
        {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
        {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
        {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
        {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
        {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
        {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
        {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
        {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
        {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
    };
    unsigned int inv_s[16][16] =
    {
        {0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},
        {0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},
        {0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},
        {0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},
        {0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},
        {0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},
        {0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},
        {0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},
        {0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},
        {0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},
        {0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},
        {0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},
        {0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},
        {0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},
        {0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},
        {0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}
    };
    protected:
    int key[44][4];
    int gf2n_multiply(int a, int b) 
    {
        const int overflow = 0x100, modul = 0x11B;  // AES GF(2^8) representation
        int sum = 0;
        while (b > 0) {
            if (b & 1) sum = sum ^ a;             // if last bit of b is 1, add a to the sum
            b = b >> 1;                           // divide b by 2, discarding the last bit
            a = a << 1;                           // multiply a by 2
            if (a & overflow) a = a ^ modul;    // reduce a modulo the AES polynomial
        }
        return sum;
    }
    void subBytes(int mat[4][4] )
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                int row = mat[i][j] / 16;
                int col = mat[i][j] % 16;
                mat[i][j] = s[row][col];
            }
        }
    }
    void subBytes( int mat[4])
    {

        for(int i=0; i<4; i++)
        {
            int row = mat[i] / 16;
            int col = mat[i] % 16;
            mat[i] = s[row][col];
        }
    }
    void InvsubBytes(int mat[4][4] )
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                int row = mat[i][j] / 16;
                int col = mat[i][j] % 16;
                mat[i][j] = inv_s[row][col];
            }
        }
    }

    void LCS( int mat[4], int numByte)
    {
        while(numByte>0)
        {
            int temp = mat[0];
            for(int i=0; i<3; i++)
            {
                mat[i]=mat[i+1];
            }
            mat[3] = temp;
            numByte--;
        }
    }
    void RCS( int mat[4], int numByte)
    {
        while(numByte>0)
        {
            int temp = mat[3];
            for(int i=3; i>0; i--)
            {
                mat[i]=mat[i-1];
            }
            mat[0] = temp;
            numByte--;
        }
    }

    int* gFunc(int w[4], int i)
    {
        int *arr = new int[4];
        for(int j=0; j<4; j++) arr[j]=w[j];

        LCS(arr,1);

        subBytes(arr);

        arr[0] = arr[0]^RCon[i-1];
        
        cout<<"g(w"<<4*i-1<<") = ";
        for(int j=0; j<4; j++) printf("%02X", arr[j]);
        cout<<endl;
        return arr;
    }
    
    void AddRoundKey( int mat[4][4], int k)
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                mat[j][i] = mat[j][i]^key[4*k+i][j];
            }
        }
    }
    void ShiftRow(int mat[4][4])
    {
        for(int i=0; i<4; i++)
        {
            LCS(mat[i],i);
        }
    }
    void InvShiftRow(int mat[4][4])
    {
        for(int i=0; i<4; i++)
        {
            RCS(mat[i],i);
        }
    }
    void MixColumns(int mat[4][4], int temp[4][4])
    {
        int col[4][4] = {{0x02,0x03,0x01,0x01},{0x01,0x02,0x03,0x01},{0x01,0x01,0x02,0x03},{0x03,0x01,0x01,0x02}};

        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                int sum=0;
                for(int k=0; k<4; k++)
                {
                    sum = sum ^ (gf2n_multiply(col[i][k],mat[k][j]));  // matrix multplication with GF(2^8);
                }
                temp[i][j] = sum;
            }
        }
    }

    void InvMixColumns(int mat[4][4], int temp[4][4])
    {
        int col[4][4] = {{0x0E,0x0B,0x0D,0x09},{0x09,0x0E,0x0B,0x0D},{0x0D,0x09,0x0E,0x0B},{0x0B,0x0D,0x09,0x0E}};
        
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                int sum=0;
                for(int k=0; k<4; k++)
                {
                    sum = sum ^ (gf2n_multiply(col[i][k],mat[k][j]));  // matrix multplication with GF(2^8);
                }
                temp[i][j] = sum;
            }
        }
    }

    public:

    void KeyGeneration(int ch[4][4])
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
                key[i][j] = ch[i][j];
        }
        for(int k=0; k<4; k++)
        {
                cout<<"W"<<k<<"= ";
                for(int j=0; j<4; j++)
                {
                    printf("%02X ", key[k][j]); 
                }
                cout<<endl;
        }
        for(int i=1;i<=10; i++)
        {
            int *temp = new int[4];
            temp = gFunc(key[(i*4)-1],i);

            for(int j=0; j<4; j++)
                key[(i*4)][j] = key[(i*4)-4][j]^temp[j];

            for(int j=0; j<4; j++)
                key[(i*4)+1][j] = key[(i*4)-3][j]^key[(i*4)][j];

            for(int j=0; j<4; j++)
                key[(i*4)+2][j] = key[(i*4)-2][j]^key[(i*4)+1][j];

            for(int j=0; j<4; j++)
                key[(i*4)+3][j] = key[(i*4)-1][j]^key[(i*4)+2][j];  
            
            for(int k=0; k<4; k++)
            {
                cout<<"W"<<4*i+k<<"= ";
                for(int j=0; j<4; j++)
                {
                    printf("%02X ", key[4*i+k][j]); 
                }
                cout<<endl;
            }
        }
    }

    void Encrypt(int plaintext[4][4], int pt[4][4])
    {
        cout<<"Intial pt\n";
        for(int k=0; k<4; k++)
        {
            for(int j=0; j<4; j++)
            {
                pt[k][j]=plaintext[k][j];
                printf("%02X ", pt[k][j]); 
            }
            cout<<endl;
        }
        AddRoundKey(pt,0);
        cout<<"Intializatio Add Round Key: \n";
        for(int k=0; k<4; k++)
        {
            for(int j=0; j<4; j++)
            {
                printf("%02X ", pt[k][j]); 
            }
            cout<<endl;
        }
        

        for(int i=1; i<=9; i++)
        {
            subBytes(pt);
            cout<<"\nRound "<<i<<"\nAfter SubBytes: \n";
            for(int k=0; k<4; k++)
            {
                for(int j=0; j<4; j++)
                {
                    printf("%02X ", pt[k][j]); 
                }
                cout<<endl;
            }
            ShiftRow(pt);
            cout<<"Shift Rows: \n";
            for(int k=0; k<4; k++)
            {
                for(int j=0; j<4; j++)
                {
                    printf("%02X ", pt[k][j]); 
                }
                cout<<endl;
            }

            int temp[4][4];
            MixColumns(pt,temp);
            for(int k=0; k<4; k++)
            {
                for(int j=0; j<4; j++)
                {
                    pt[k][j]=temp[k][j];
                }

            }

            cout<<"After Mix Columns: \n";
            for(int k=0; k<4; k++)
            {
                for(int j=0; j<4; j++)
                {
                    printf("%02X ", pt[k][j]); 
                }
                cout<<endl;
            }
            AddRoundKey(pt,i);
            cout<<"After AddRound: \n";
            for(int k=0; k<4; k++)
            {
                for(int j=0; j<4; j++)
                {
                    printf("%02X ", pt[k][j]); 
                }
                cout<<endl;
            }
        }

        subBytes(pt);
        cout<<"\nRound "<<10<<"\nAfter SubBytes: \n";
        for(int k=0; k<4; k++)
        {
            for(int j=0; j<4; j++)
            {
                printf("%02X ", pt[k][j]); 
            }
            cout<<endl;
        }

        ShiftRow(pt);
        cout<<"Shift Rows: \n";
        for(int k=0; k<4; k++)
        {
            for(int j=0; j<4; j++)
            {
                printf("%02X ", pt[k][j]); 
            }
            cout<<endl;
        }

        AddRoundKey(pt,10);
        cout<<"After AddRound: \n";
        for(int k=0; k<4; k++)
        {
            for(int j=0; j<4; j++)
            {
                printf("%02X ", pt[k][j]); 
            }
            cout<<endl;
        }

        cout<<"Encrypted text: ";
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                printf("%02X ",pt[i][j]);
            }
        }
        cout<<endl;

    }
    
    void Decrypt(int pt[4][4])
    {
        cout<<"Decrytion\nIntial Cipher Text \n";
        for(int k=0; k<4; k++)
        {
            for(int j=0; j<4; j++)
            {
                printf("%02X ", pt[k][j]); 
            }
            cout<<endl;
        }

        AddRoundKey(pt,10);
        cout<<"After Initial AddRound: \n";
        for(int k=0; k<4; k++)
        {
            for(int j=0; j<4; j++)
            {
                printf("%02X ", pt[k][j]); 
            }
            cout<<endl;
        }
        int r=1;
        for(int i=9; i>=1; i--)
        {

            InvShiftRow(pt);
            cout<<"\nRound "<<r++<<"\n Inv Shift Rows: \n";
            for(int k=0; k<4; k++)
            {
                for(int j=0; j<4; j++)
                {
                    printf("%02X ", pt[k][j]); 
                }
                cout<<endl;
            }


            InvsubBytes(pt);
            cout<<"After InvSubBytes: \n";
            for(int k=0; k<4; k++)
            {
                for(int j=0; j<4; j++)
                {
                    printf("%02X ", pt[k][j]); 
                }
                cout<<endl;
            }

            AddRoundKey(pt,i);
            cout<<"After AddRound: \n";
            for(int k=0; k<4; k++)
            {
                for(int j=0; j<4; j++)
                {
                    printf("%02X ", pt[k][j]); 
                }
                cout<<endl;
            }


            int temp[4][4]; 
            InvMixColumns(pt,temp);
            for(int k = 0; k<4; k++)
            {
                for(int j=0; j<4; j++)
                {
                    pt[k][j] = temp[k][j];
                }
            }
           cout<<"After Inv Mix Columns: \n";
            for(int k=0; k<4; k++)
            {
                for(int j=0; j<4; j++)
                {
                    printf("%02X ", pt[k][j]); 
                }
                cout<<endl;
            }
        }

        InvShiftRow(pt);
        cout<<"\nRound 10 \nInv Shift Rows: \n";
        for(int k=0; k<4; k++)
        {
            for(int j=0; j<4; j++)
            {
                printf("%02X ", pt[k][j]); 
            }
            cout<<endl;
        }

        InvsubBytes(pt);
        cout<<"After InvSubBytes: \n";
        for(int k=0; k<4; k++)
        {
            for(int j=0; j<4; j++)
            {
                printf("%02X ", pt[k][j]); 
            }
            cout<<endl;
        }
        
        AddRoundKey(pt,0);
        cout<<"After AddRound: \n";
        for(int k=0; k<4; k++)
        {
            for(int j=0; j<4; j++)
            {
                printf("%02X ", pt[k][j]); 
            }
            cout<<endl;
        }

        cout<<"Decrypted text: ";
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                printf("%02X ",pt[j][i]);
            }
        }
    }
};

class CipherMode
{
    
    public:
    int datatype;
    string plaintext, key;
    int pt[4][4], ke[4][4];

    void MenuDriven()
    {

        while(true)
        {
            int n;
            cout<<"\n__________Chose the Block Cipher________"<<endl;
            cout<<"1. S-DES 2. AES-128 3. Exit"<<endl;
            cin>>n;
            if(n==3) break;
            
            L1: cout<<"Choose the data type 1. Binary 2.Hexadecimal: "; 
            cin>>datatype;
            if(datatype != 1 && datatype != 2) 
            {
                cout<<"choose approproate data type :("<<endl;
                goto L1;
            }

            if(n==1)
            {
                if(datatype==1) TakeBinaryInputSDES();
                else TakeHexInputSDES();
                SDES obj;
                obj.KeyGeneration(key);
                string ct = obj.Encrypt(plaintext);
                cout<<"Cipher text is: "<<ct<<endl<<endl;
                int d;
                cout<<"Enter 1 to Decrypt else press any: ";
                cin>>d;
                if(d==1)
                {
                    string dt = obj.Decrypt(ct);
                    cout<<"Decrypted text is: "<<dt<<endl<<endl;
                }
            }
            else if(n==2)
            {
                if(datatype==1) TakeBinaryInputAES();
                else TakeHexInputAES();
                AES a;
                int cipher[4][4];
                a.KeyGeneration(ke);
                a.Encrypt(pt,cipher);

                int d;
                cout<<"Enter 1 to Decrypt else press any: ";
                cin>>d;
                if(d==1)
                {
                    a.Decrypt(cipher);
                }
               
            }
        }
    }

    void TakeBinaryInputSDES()
    {
        cout<<"Enter binary plain text: ";
        cin>>plaintext;
        cout<<"Enter binary key text: ";
        cin>>key;
        if(!(validateBin(plaintext,8)&&validateBin(key,10)))
        {
            cout<<"Data Not Validated :(!"<<endl;
            TakeBinaryInputSDES();
        } 
        else cout<<"Data Validated!!!"<<endl;
        
    }
    void TakeBinaryInputAES()
    {
        cout<<"Enter binary plain text: ";
        cin>>plaintext;
        cout<<"Enter binary key text: ";
        cin>>key;
        if(!(validateBin(plaintext,128)&&validateBin(key,128)))
        {
            cout<<"Data Not Validated :(("<<endl;
            TakeBinaryInputAES();
        } 
        else
        {
            cout<<"Data Validated!!!"<<endl;
            int k=0;
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    pt[j][i] = stoi(plaintext.substr(k,8),nullptr,2);
                    ke[i][j] = stoi(key.substr(k,8),nullptr,2);
                    k = k+8;
                }
            }
            plaintext="";key="";
        }

    }
    void TakeHexInputSDES()
    {
        cout<<"Enter hex plain text: ";
        cin>>plaintext;
        cout<<"Enter hex key text: ";
        cin>>key;
        if(!(validadeHex(plaintext,2) && validadeHex(key,3)))
        {
            cout<<"Data Not Validated :(!"<<endl;
            TakeHexInputSDES();
        }
        plaintext = hexToBinary8(plaintext);
        key = hexToBinary10(key);
        if(!(validateBin(plaintext,8)&&validateBin(key,10)))
        {
            cout<<"Data Not Validated :(!"<<endl;
            TakeHexInputSDES();
        }   
        else cout<<"Data Validated!!!"<<endl;
    }
    void TakeHexInputAES()
    {
        cout<<"Enter hex plain text: ";
        cin>>plaintext;
        cout<<"Enter hex key text: ";
        cin>>key;
        if(!(validadeHex(plaintext,32) && validadeHex(key,32)))
        {
            cout<<"Data Not Validated :("<<endl;
            TakeHexInputAES();
        }
        else
        {
            cout<<"Data Validated!!!"<<endl;
            int k=0;
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    pt[j][i] = stoi(plaintext.substr(k,2),nullptr,16);
                    ke[i][j] = stoi(key.substr(k,2),nullptr,16);
                    k = k+2;
                }
            }
        }
        plaintext="";key="";
    }

    bool validadeHex(string &str,int num)
    {
        while( str.size() < num) str = '0'+str;
        int n = str.size();
        if(n>num) return false;

        for(int i=0; i<num; i++)
        {
            if(isdigit(str[i])) continue;
            else if( str[i]=='A' || str[i]=='a' || str[i]=='B' ||str[i]=='b' ||str[i]=='C' ||str[i]=='c' ||str[i]=='D'|| str[i]=='d' ||str[i]=='E'||str[i]=='e' ||str[i]=='F' ||str[i]=='f')
                continue;
            else return false;
        }
        return true; 
    }
    

    string hexToBinary8(string hexString) 
    {
        int hexValue=0;
        string binaryString;
        reverse(hexString.begin(), hexString.end());
        for (int i=0; i<hexString.size(); i++) {
            
            if (hexString[i] >= '0' && hexString[i] <= '9') {
                hexValue += pow(16,i)*(hexString[i] - '0');
            } else if (hexString[i] >= 'a' && hexString[i] <= 'f') {
                hexValue += pow(16,i)*(hexString[i] - 'a' + 10);
            } else if (hexString[i] >= 'A' && hexString[i] <= 'F') {
                hexValue += pow(16,i)*(hexString[i] - 'A' + 10);
            } else {
                continue;
            }
        }
        binaryString = std::bitset<8>(hexValue).to_string();
        return binaryString;
    }
    string hexToBinary10(string &hexString) 
    {
        int hexValue=0;
        string binaryString;
        reverse(hexString.begin(), hexString.end());
        for (int i=0; i<hexString.size(); i++) {
            
            if (hexString[i] >= '0' && hexString[i] <= '9') {
                hexValue += pow(16,i)*(hexString[i] - '0');
            } else if (hexString[i] >= 'a' && hexString[i] <= 'f') {
                hexValue += pow(16,i)*(hexString[i] - 'a' + 10);
            } else if (hexString[i] >= 'A' && hexString[i] <= 'F') {
                hexValue += pow(16,i)*(hexString[i] - 'A' + 10);
            } 
        }
        binaryString = std::bitset<10>(hexValue).to_string();
        return binaryString;
    }

    bool validateBin(string &str, int num)
    {
        while( str.size() < num) str = '0'+str;
        int n = str.size();
        if(n>num) return false;

        for(int i=0; i<num; i++)
        {
            if(str[i] != '1' && str[i] !='0') return false; 
        }
        return true; 
    }
};

    
int main()
{
    CipherMode b; 
    b.MenuDriven();
}

