//const int cMaxNumOfEvents = 11549773;
const int cMaxNumOfBins = 100000;
const unsigned short cNeutronChannel = 0;
const unsigned short cTriggerChannel = 1;
const unsigned short cFlipperChannel = 2;
const unsigned short cSuperChannel   = 3;

struct tWord {
    unsigned char firstbyte;
    unsigned char secondbyte;
};

struct tThreeword {
    tWord lo;
    tWord mid;
    tWord hi;
};

unsigned long long threeword2ull(tThreeword threeword) {
   unsigned long long ergebnis;
   ergebnis = threeword.hi.firstbyte;
   ergebnis = (ergebnis << 8) + threeword.hi.secondbyte;
   ergebnis = (ergebnis << 8) + threeword.mid.firstbyte;
   ergebnis = (ergebnis << 8) + threeword.mid.secondbyte;
   ergebnis = (ergebnis << 8) + threeword.lo.firstbyte;
   ergebnis = (ergebnis << 8) + threeword.lo.secondbyte;
   return ergebnis;
}

int str2int(char* pString) {
    char c;
    int i;
    int result;
    int sign;
    i = 0;
    result = 0;
    c = pString[0];
    sign = 1;
    if (c == '+') {
        sign = +1;
        i++;
    }
    if (c == '-') {
        sign = -1;
        i++;
    }
    c = pString[i];
    while(c != (char)0) {
        result *= 10;
        result += (int)c - 48;
        i++;
        c = pString[i];
    }
    result *= sign;
    return result;
}

float str2float(char* pString) {
    char c;
    int i;
    float result;
    float factor;
    int sign;
    i = 0;
    result = 0.0;
    c = pString[0];
    sign = 1;
    if (c == '+') {
        sign = +1;
        i++;
    }
    if (c == '-') {
        sign = -1;
        i++;
    }
    c = pString[i];
    while(c != '.') {
        result *= 10.0;
        result += (float)c - 48;
        i++;
        c = pString[i];
    }
    i++;
    c = pString[i];
    factor = 1.0;
    while(c != (char)0) {
        factor = factor * 0.1;
        result += (float(c) - 48) * factor;
        i++;
        c = pString[i];
    }
    result *= sign;
    return result;
}
