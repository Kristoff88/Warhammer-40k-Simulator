
def LoadOBJ(filename = "empty"):

    result = CheckFilenameCorrectness(filename)
    if( result != "Correct" ):
        return result

    
    try:
        OBJfile = open(filename, 'r')

    except IOError:
        result = "LoadOBJ(filename) Error:\Incorrect filename has been given!\n\n" + filename
        return result
    

    result = ReadOBJFile( OBJfile )
    return result


###############################################################################

def CheckFilenameCorrectness(filename):

    if filename == "empty":
        result = "LoadOBJ(filename) Error:\n\nEmpty filename!"
        return result

    if not isinstance(filename, str):
        result = "LoadOBJ(filename) Error:\nIncorrect argument: " + str(filename) + "\nArgument powinien byc stringiem!"
        return result

    return "Correct"


def ReadOBJFile(OBJfile):

    # --------------------------------------------------
    vertexCount = 0;  texCoordCount = 0;  normalCount = 0
    vertexArray = []; texCoordArray = []; normalArray = []

    textureCount = 0
    textureArray = []
    
    faceCount = 0
    faceArray = []
    #ReceivedFace = False
    LoadingFirstFace = True
    
    indexCountersCount = 0
    indexCountersArray = []
    newIndexCounter    = 0
    # --------------------------------------------------

    rowCounter = -1
    EOF = 0
    
    while True:

        line = OBJfile.readline()
        
        if len(line) == EOF:
            break; 

        else:
            words = str.split(line)
            rowCounter = rowCounter + 1
     
            if words[0] == 'v':
                vertexArray.append( GetValue(words, "vertex", 3) )
                vertexCount = vertexCount + 1
        
            if words[0] == 'vt':
                texCoordArray.append( GetValue(words, "texture coordinate", 2) )
                texCoordCount = texCoordCount + 1

            if words[0] == 'vn':
                normalArray.append( GetValue(words, "normal", 3) )
                normalCount = normalCount + 1

         
            if words[0] == 'usemtl':
                
                if words[-1] == 'usemtl':
                    result = Error(filename, rowCounter)
                    result += "Found empty 'usemtl' statement."
                    return result
                
                else:
                    textureArray.append(words[1]);
                    textureCount = textureCount + 1

                    if LoadingFirstFace == True:
                        LoadingFirstFace = False

                    else:
                        indexCountersArray.append(newIndexCounter)
                        indexCountersCount = indexCountersCount + 1
                        newIndexCounter = 0

                    
            if words[0] == 'f':
                
                newFace = []

                try:
                    newFace = [ words[1].split('/'), words[2].split('/'), words[3].split('/') ]
                    faceArray.append(newFace)
                    faceCount = faceCount + 1

                except IndexError:
                    "Incorrect word number in face description (row: " + str(rowCounter) + ")"
                    return result
                
                newIndexCounter = newIndexCounter + 3
                #ReceivedFace = True
            
##            elif ReceivedFace == True:
##            
##                indexCountersArray.append(newIndexCounter)
##                indexCountersCount = indexCountersCount + 1
##                
##                newIndexCounter = 0
##                ReceivedFace = False

        
    # End of While loop.
    
    
##    if indexCountersCount == 0:

    indexCountersArray.append(newIndexCounter)
    indexCountersCount = indexCountersCount + 1
        
        

    
    OBJfile.close()

    
    if texCoordCount == 0:
        result = Error(filename, rowCounter)
        result += "Model doesn't have any texture coordinates!"
        return result
        


    result = [ vertexCount,   vertexArray,   \
               texCoordCount, texCoordArray, \
               normalCount,   normalArray,   \
                    
               textureCount, textureArray,   \
               faceCount,    faceArray,      \
               
               indexCountersCount,  indexCountersArray ]


##    print "Vertices (" + str(vertexCount) + "):\n"
##    print vertexArray
##    print "\n\n"
##
##    print "TexCoords (" + str(texCoordCount) + "):\n"
##    print texCoordArray
##    print "\n\n"
##
##    print "Normals (" + str(normalCount) + "):\n"
##    print normalArray
##    print "\n\n"
##
##
##    print "Textures (" + str(textureCount) + "):\n"
##    print textureArray
##    print "\n\n"
##
##    print "Faces (" + str(faceCount) + "):\n"
##    print faceArray
##    print "\n\n"
##
##    print "Indices (" + str(indexCountersCount) + "):\n"
##    print indexCountersArray
##    print "\n\n"

    


    return result

###############################################################################


def Error(filename, rowNumber):

    result = "Error while loading OBJ file: " + filename + "\n"
    result += "Row: " + str(rowNumber) + "\n\n"
    return result


def GetValue(words, valueName, expectedWordCount):

    newValue = []

    try:
        index = 0;

        while index < expectedWordCount:
            newValue.append(words[index+1])
            index = index+1

        return newValue

    except IndexError:
        result = Error(filename, rowCounter)
        result += "Incorrect words number in " + str(valueName) + " description."
        return result

    
