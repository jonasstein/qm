#! /usr/bin/python3
import construct as cstr



def UBInt48(name):
    return cstr.String(name, 6)

def main():
    myfile = open('./qmlist/3kHz_1kHz_FM_noise_1800sec.mdat', 'rb')
    substrate = myfile.read()
    myfile.close()
    
    line = cstr.CString("line", terminators = b'\x0a')
    
    header_separator = cstr.String("foo", 8)

    header = cstr.Sequence( \
     "header", \
     line, \
     cstr.String("foo", 15), \
     cstr.CString("length", terminators = b' '), \
     cstr.Array(lambda ctx: int(ctx.length) - 1, line), \
     header_separator)

    buffer_separator = cstr.String("buffer_separator", 8)

    buffer_header = cstr.Struct( \
     "buffer_header", \
     cstr.UBInt16("buffer_type"), \
     cstr.UBInt16("header_length"), \
     cstr.UBInt16("buffer_number"), \
     cstr.UBInt16("run_id"), \
     cstr.UBInt8("mcpd-id"), \
     cstr.UBInt8("status"), \
     UBInt48("a"), \
     UBInt48("b"), \
     UBInt48("c"), \
     UBInt48("d"), \
     UBInt48("e"))

    buffer_itself = cstr.Struct( \
     "buffer_itself", \
     cstr.UBInt16("length"), \
     buffer_header, \
     cstr.Array(lambda ctx: (int(ctx.length) - 21) / 3.0, \
      cstr.String("events", 6)))

    buffer = cstr.Struct( \
     "buffer", \
     buffer_itself, \
     buffer_separator)


    body = cstr.OptionalGreedyRange(buffer)

    everything = cstr.Struct( \
     "everything", \
     header, \
     body)
    ergebnis = everything.parse(substrate)
    return ergebnis

 
if __name__ == '__main__':
    main()
