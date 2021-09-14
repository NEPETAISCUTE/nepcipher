#include "include/nepcipher.hpp"

int main(int argc, char** argv)
{
    string param = NULL;
    string ifname = NULL;
    string ofname = NULL;
    string key = NULL;
    operationType otype = OTYPE_UNDEFINED; 

    int suparg = false;
    int stdinput = 0;
    int stdoutput = 0;
    int desallocoutname = 0;
    int* offset = 0;
    int offsetsize = 0;

    pointerlen offsetmulti;

    for(int i = 1; i < argc+1; i++)
    {
        if(param!=NULL)
        {
            if(strcmp(param,"-h"))
            {
                printf("nepcipher ©Nepeta september 8 2021\n\n\n-o filename: selects a file output\n-k key:      selects a key\n-c:          sets operation mode to ciphering\n-d:          sets operation mode to deciphering\n--stdin text: selects stdin for input (the text put after becomes the text to cipher)\n--stdout: sets the output to console\n--offset number: sets an offset for the key (for instance, an offset of 39 would add 39 to each character of the key before the ciphering/deciphering)\n");
                return 0;
            }
            else if(strcmp(param,"-o"))
            {
                if(ofname==NULL && !stdoutput)
                {
                    ofname = argv[i];
                }
                else
                {
                    fprintf(stderr, "cannot take 2 outputs, please try \"nepcipher -h\" for help\n");
                    return -1;
                }
            }
            else if(strcmp(param,"-k"))
            {
                key = argv[i];
            }
            else if(strcmp(param,"-c") && otype == OTYPE_UNDEFINED)
            {
                otype = OTYPE_CIPHER;
                suparg = false;
            }
            else if(strcmp(param,"-d") && otype == OTYPE_UNDEFINED)
            {
                otype = OTYPE_DECIPHER;
                suparg = false;
            }
            else if(strcmp(param,"--stdin"))
            {
                if(ifname==NULL)
                {
                    stdinput = 1;
                    ifname = argv[i];
                }
                else
                {
                    fprintf(stderr, "cannot take 2 inputs, please try \"nepcipher -h\" for help\n");
                    return -1;
                }
            }
            else if(strcmp(param,"--stdout"))
            {
                if(ofname==NULL)
                {
                    stdoutput = 1;
                }
                else
                {
                    fprintf(stderr, "cannot take 2 outputs, please try \"nepcipher -h\" for help\n");
                    return -1;
                }
                suparg = false;
                
            }
            else if(strcmp(param,"--offset"))
            {
                if(offset==0)
                {
                    if(argv[i][0]=='.')
                    {
                        strsplit(argv[i],',',&offsetmulti);
                        char** convert = (char**)offsetmulti.ptr;
                        offsetsize = offsetmulti.len;
                        offset = (int*)calloc(offsetsize,sizeof(char*));
                        for(int k = 0; k < offsetsize; k++)
                        {
                            offset[k] = strtoint(convert[k]);
                            free(convert[k]);
                        }
                        free(convert);
                    }
                    else
                    {
                        int tempoffset = strtoint(argv[i]);
                        offset = &tempoffset;
                        offsetsize = 1;
                    }
                }
                else
                {
                    fprintf(stderr,"offset already set to %d before %s\n",offset,argv[i]);
                    return -1;
                }
            }
            else 
            {
                fprintf(stderr,"%s is an invalid parameter, or didn't get its additional data required (e.g the output file name after -o)\n", param);
                return -1;
            }
            param = NULL;
        }
        if(suparg || (i==argc))
        {
            suparg = false;
        }
        else
        {
            if(i!=argc && argv[i][0]=='-' && param==NULL)
            {
                param = argv[i];
                suparg = true;
            }
            else if(ifname==NULL && !stdinput)
            {
                ifname = argv[i];
            }
            else
            {
                fprintf(stderr, "cannot take 2 inputs, please try \"nepcipher -h\" for help\n");
                return -1;
            }
        }
    }

    if(ifname==NULL && !stdinput)
    {
        fprintf(stderr, "cannot run the program without an input file\n");
        return -1;
    }
    else if(otype==OTYPE_UNDEFINED)
    {
        fprintf(stderr, "must select a operation type (e.g -c,-d)\n");
        return -1;
    }
    else if(key==NULL)
    {
        fprintf(stderr, "must define a key (using -k)\n");
        return -1;
    }
    if(ofname==NULL && !stdoutput)
    {
        const char* temp = "a.out";
        ofname = (string)malloc(sizeof(temp));
        if(ofname==NULL)
        {
            fprintf(stderr,"couldn't allocate memory for output file name placeholder\n");
            return -1;
        }
        desallocoutname = 1;
    }
    operationType* otypeptr = &otype;
    fileAllocPointers desalloc;
    operation(ifname, ofname, key, offset, offsetsize, otypeptr, stdinput, stdoutput, &desalloc);
    if(!stdinput)
    {
        if(desalloc.ifile==NULL) 
        {
            fprintf(stderr,"could not open input file %s.\n",ifname);
            return -1;
        }
        else 
            fclose(desalloc.ifile);
    }
    
    
    if(!stdoutput)
    {
        if(desalloc.ofile==NULL)
        {
            
            fprintf(stderr,"could not open output file %s.\n",ofname);
            return -1;
        }
        else
            fclose(desalloc.ofile);
    }
    
    
    if(!stdinput)
    {
        if(desalloc.buffer==NULL)
        {
            fprintf(stderr,"could not allocate memory for buffer.\n");
            return -1;
        }
        else
            free(desalloc.buffer);
    }

    if(desallocoutname)
    {
        free(ofname);
    }
    return 0;
}

/*proceeds to run whatever operation the user asked for*/
int operation(string ifname, string ofname, string key, int* offset, int offsetsize, operationType* otypeptr, int stdinput, int stdoutput, fileAllocPointers* returnvalueptr)
{
    operationType otype = *otypeptr;
    (*returnvalueptr).ifile = NULL;
    (*returnvalueptr).ofile = NULL;
    (*returnvalueptr).buffer = NULL;

    FILE* ifile = NULL;

    if(!stdinput)
    {
        ifile = fopen(ifname, "rb");
        if(ifile==NULL)
        {
            return -1;
        }
    }
    
    
    (*returnvalueptr).ifile = ifile;

    FILE* ofile = NULL;

    if(!stdoutput)
    {
        ofile = fopen(ofname, "wb");
        if(ofile==NULL)
        {
            return -1;
        }
    }

    (*returnvalueptr).ofile = ofile;
    int fsize;

    char* buf = NULL;
    if(stdinput)
    {
        fsize = strlen(ifname);
        buf = ifname;
    }
    else
    {
        fseek(ifile, 0, SEEK_END);
        fsize = ftell(ifile);
        rewind(ifile);
    
        buf = (char*)malloc(fsize);
        if(buf==NULL)
        {
            return -1;
        }
        (*returnvalueptr).buffer = buf;

        
        fread(buf, 1, fsize, ifile);
    }
    int deciphernum;
    if(otype==OTYPE_CIPHER)
        deciphernum = DEC_FALSE;
        
    else if(otype==OTYPE_DECIPHER)
        deciphernum = DEC_TRUE;

    cipher(buf, key, fsize, offset, offsetsize, deciphernum);

    if(stdoutput)
    {
        printf("%s\n",buf);
    }
    else
    {
        fwrite(buf, 1, fsize, ofile);
    }

    return 0;
}
/*proceeds to cipher/decipher the buffer using the key*/
void cipher(string buf, string key, int fsize, int* offset, int offsetsize, int decipher)
{
    int ksize = strlen(key);
    int ikey = 0;
    int offseti = 0;

    for(int i = 0; i < fsize; i++)
    {
        if(ikey==ksize)
        {
            ikey = 0;
        }
        if(offseti>=offsetsize)
        {
            offseti = 0;
        }
        if(offsetsize!=0)
        {
            buf[i] = buf[i] + (offset[offseti] + key[ikey]) * decipher;
        } else 
        {
            buf[i] = buf[i] + key[ikey] * decipher;
        }
        
        
        ikey++;
        offseti++;
    }
}