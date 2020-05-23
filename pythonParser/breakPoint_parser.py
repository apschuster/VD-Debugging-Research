#The point of this script is to be used with the SetFuncBPS to extract the local variables/argument
#names from each breakpoint, and then create a .gdb file (That is sourced within the SetFuncBPS) that changes those values.
#At the moment it only changes them to '1', but a future student can work on modifying this script
#To change the values to whatever they want using hard coded values, or taking input files with new values
#to change these variables to.
import re

def FileToString(filePath):
    try:
        with open(filePath, 'r') as file:
            return file.read()
    except IOError:
        sys.stderr.write("Error: failed to open \'%s\' for reading.\n" % filePath)
        exit(1)

def writeGdbFile(arguments, localVars, fileName, flag):
    try:
        with open(fileName, 'w') as file:
            if(flag==1):
                for argument in arguments:
                    file.write('set ')
                    file.write(argument)
                    file.write('=1')
                    file.write('\n')
            else:
                for argument in arguments:
                    file.write('set ')
                    file.write(argument)
                    file.write('=1')
                    file.write('\n')
                for variable in localVars:
                    file.write('set ')
                    file.write(variable)
                    file.write('=1')
                    file.write('\n')

    except IOError:
        sys.stderr.write("Error: failed to open \'%s\' for writing.\n" % fileName)
        exit(1)



def main():
    fileString= FileToString('gdb.txt')
    #Find all istances from thread to two new line characters in a row
    #this should be all the distinct breakpoint matches to this point.
    listStrings= re.findall(r'Thread(?:(?!\n\n).)*\n\n', fileString, re.DOTALL)
    
    #Looking for latest instance of breakpoint information (Has not been able to be perfect against actual logging file yet)
    lastCapture= listStrings[len(listStrings)-1]
    #Detect how many arguments there are and place them in a list
    arguments= lastCapture[lastCapture.find("(")+1:lastCapture.find(")")].split(',')
    localVars= []
    #separate each line within the string
    splitLines= lastCapture.split('\n')
    count=1
    for line in splitLines:
        #look for all occurences of local variables, stop searching once known argument is
        #found.
        if(line.replace(' ','') == arguments[0].replace(' ','') and count>2):
            break
        elif(count>2):
            localVars.append(line)

        count+=1

    #Now want to extract (parse) only the argument/local variables names, not the values
    #This is so we know what names to change.
    i=0
    for argument in arguments:
        arguments[i]= argument[0:argument.find('=')]
        arguments[i]= arguments[i].replace(' ','')
        i+=1
    i=0
    for variable in localVars:
        localVars[i]= variable[0:variable.find('=')]
        localVars[i]=localVars[i].replace(' ','')
        i+=1

    #print(arguments)
    #print(localVars)
    flag=0
    #Check if there are local variables or not, will change file writing process
    if(localVars[0]=='Nolocals'):
        flag=1
    #What we want to name the output .gdb file.
    fileName='changeVars.gdb'
    #create a .gdb file to source that sets all the variables for that breakpoint to a new value
    #At the moment it sets all the values to '1' but this is something that can be experimented with
    writeGdbFile(arguments,localVars,fileName,flag)


if __name__ == "__main__":
    main()