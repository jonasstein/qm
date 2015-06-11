const int cMaxNumOfEvents = 11549773;
const int cMaxNumOfBins = 10000;
const unsigned short cTriggerChannel = 1;

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
    int nachkomma;
    char c;
    int i;
    float result;
    float factor;
    int sign;
    nachkomma = 0;
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
