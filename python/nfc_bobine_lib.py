import re
import base64

def which_slicer(file_choiced):
    
    file_choiced1 = open(file_choiced, "r")
    lines = file_choiced1.readlines()
    for line in lines :
        if (re.search("generated", line, re.IGNORECASE)):
            slicer = line
            break
        else :
            slicer = ""
    
    if (re.search("Prusa", slicer, re.IGNORECASE)) :
        print("Seems this file was generated with Prusa\n")
        slicer = "Prusa"
    elif (re.search("Super", slicer, re.IGNORECASE)) :
        print("Seems this file was generated with Slic3r\n")
        slicer = "Super"
    elif (re.search("idea", slicer, re.IGNORECASE)) :
        print("Seems this file was generated with IdeaMake\nr")
        slicer = "Idea"
    elif (re.search("Orca", slicer, re.IGNORECASE)) :
        print("Seems this file was generated with Orca\n")
        slicer = "Orca"
    elif(re.search("cura", slicer, re.IGNORECASE)):
        print("Seems this file was generated with Cura\n")
        slicer = "Cura"
    else :
        print("Slicer = ",slicer)

    return slicer

def extacting_data(file_choiced):

    file_choiced1 = open(file_choiced, "r")
    lines = file_choiced1.readlines()
    x = len(lines)
    print(x)
    count = 0
    while True :
        if count >= x :
            break
        
        if (lines[count].find("filament_type =")>0) :
            #; filament_type = PLA
            filament_type = lines[count][-4:-1]
            print("filament_type :",filament_type )
            #list.append(filament_type)
            count += 1
            continue
        elif (lines[count].find("filament_colour =")>0) :
            #; filament_colour = #428AF5
            filament_colour = lines[count][-8:-1]
            print("filament_colour :",filament_colour )
            #list.append(filament_colour)
            count += 1
            continue
        elif (lines[count].find("filament used [mm] =")>0) :
            #; filament used [mm] = 515.50
            filament_used_mm = lines[count][-7:-1]
            print("filament_used_mm :",filament_used_mm )
            #list.append(filament_used_mm)
            count += 1
            continue
        elif (lines[count].find("total filament used [g] = ")>0) :
            #; filament used [g] = 1.50
            filament_used_g = lines[count][-5:-1]
            print("filament_used_g :",filament_used_g ) 
            #list.append(filament_used_g)
            count += 1
            continue
        elif (lines[count].find(" bed_temperature =")>0) :
            #; bed_temperature = 65
            bed_temperature = lines[count][-4:-1]
            print("bed_temperature :",bed_temperature ) 
            #list.append(bed_temperature)
            count += 1
            continue
        elif (lines[count].find(" temperature =")>0) :
            #; temperature = 200
            temperature = lines[count][-4:-1]
            print("temperature :",temperature ) 
            #list.append(temperature)
            count += 1
            continue
        elif (lines[count].find("first_layer_bed_temperature =")>0) :
            #; first_layer_bed_temperature = 70
            first_layer_bed_temperature = lines[count][-4:-1]
            print("first_layer_bed_temperature :",first_layer_bed_temperature )
            #list.append(first_layer_bed_temperature)
            count += 1
            continue
        elif (lines[count].find("first_layer_temperature =")>0) :
            #; first_layer_temperature = 205
            first_layer_temperature = lines[count][-4:-1]
            print("first_layer_temperature :",first_layer_temperature )
            #list.append(first_layer_temperature)
            count += 1
            continue
        count += 1
    return list

def thumbernail_small(lines):

    return

def thumbarnail_large(file_choiced):
    print(file_choiced)
    file_choiced1 = open(file_choiced, "r")
    lines = file_choiced1.readlines()
    x = len(lines)
    print("thumbarnail function")
    read_line = False
    img_file_choiced = open('temp1.txt', 'w')
    count = 0
    '''
    while True :
        if count >= x :
            break
    '''
    for line in lines :
        if (line.find("begin 400x300")>0 ) : # 21284 is numbers of chars per image 400x300 and read_line == False
            print("thumbnail begin 400x300 21284")
            print(line)
            read_line = True
        elif (line.find("thumbnail e")>0 and read_line == True) : # and read_line == True
            print("thumbnail end")
            print(line)
            read_line = False
            img_file_choiced.close()
            break
        else :
            if read_line == True :
                dec_tmp = line.replace("; ", "")
                #dec = decoded_data=base64.b64decode(dec_tmp))
                img_file_choiced.write(dec_tmp)
                print(dec_tmp)
    img_file_choiced.close()
    file = open('temp.txt', 'rb')
    encoded_data = file.read() #print(encoded_data)
    file.close()
    #decode base64 string data
    decoded_data=base64.b64decode((encoded_data))
    #write the decoded data back to original format in  file
    img_file = open('image1.png', 'wb')
    img_file.write(decoded_data)
    img_file.close()
    return

def uid_bobine():
    pass
    
def nfctag_data():
    pass

def data_from_gcode():
    pass
    
if __name__ == "__main__":
    print("this is a library")
