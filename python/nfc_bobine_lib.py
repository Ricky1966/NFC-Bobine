import re
import base64
import os

def which_slicer(lines):
    slicer = ''
    for line in lines :
        if re.search("generated", line, re.IGNORECASE) or re.search("sliced", line, re.IGNORECASE):
            slicer = line
            break

    if (re.search("PrusaSlicer", slicer)) :
        print("Seems this file was generated with Prusa\n")
        slicer = "Prusa"
    elif (re.search("SuperSlicer", slicer)) :
        print("Seems this file was generated with Slic3r\n")
        slicer = "Super"
    elif (re.search("ideaMaker", slicer)) :
        print("Seems this file was generated with IdeaMake\nr")
        slicer = "Idea"
    elif (re.search("OrcaSlicer", slicer)) :
        print("Seems this file was generated with Orca\n")
        slicer = "Orca"
    elif(re.search("Cura_SteamEngine", slicer)):
        print("Seems this file was generated with Cura\n")
        slicer = "Cura"

    return slicer

def extracting_data(lines):
    data = {'filament_type':'', 'filament_colour':'', 'filament_lenght':'', 'filament_weight':'', 'bed_temperature':'', 'temperature':'', 'first_layer_bed_temperature':'', 'first_layer_temperature':''}
    for line in lines:
        if (line.find("filament_type =")>0) :
            #; filament_type = PLA
            info = line.split('= ')[-1][:-1]
            print("filament_type :",info )
            data['filament_type'] = info
        elif (line.find("filament_colour =")>0) :
            #; filament_colour = #428AF5
            info = line.split('= ')[-1][:-1]
            print("filament_colour :",info )
            data['filament_colour'] = info
        elif (line.find("filament used [mm] =")>0) :
            #; filament used [mm] = 515.50
            info = line.split('= ')[-1][:-1]
            print("filament_used_mm :",info )
            data['filament_lenght'] = info
        elif (line.find("Filament used:")>0):
            info = line.split(': ')[-1][:-1]
            print("filament_used_mm :",info*1000 )
            data['filament_lenght'] = info*1000

        elif (line.find("total filament used [g] = ")>0) :
            #; filament used [g] = 1.50
            info = line.split('= ')[-1][:-1]
            print("filament_used_g :",info ) 
            data['filament_weight'] = info
        elif (line.find(" bed_temperature =")>0) :
            #; bed_temperature = 65
            info = line.split('= ')[-1][:-1]
            print("bed_temperature :",info) 
            data['bed_temperature'] = info
        elif (line.find(" temperature =")>0 or line.find("nozzle_temperature =")>0):
            #; temperature = 200
            info = line.split('= ')[-1][:-1]
            print("temperature :",info ) 
            data['temperature'] = info
        elif (line.find("first_layer_bed_temperature =")>0) :
            #; first_layer_bed_temperature = 70
            info = line.split('= ')[-1][:-1]
            print("first_layer_bed_temperature :",info )
            data['first_layer_bed_temperature'] = info
        elif (line.find("first_layer_temperature =")>0) :
            #; first_layer_temperature = 205
            info = line.split('= ')[-1][:-1]
            print("first_layer_temperature :",info )
            data['first_layer_temperature'] = info
    return data

def thumbarnail_small(file_choiced):
    print(file_choiced)
    file_choiced1 = open(file_choiced, "r")
    lines = file_choiced1.readlines()
    x = len(lines)
    print("thumbarnail function")
    read_line = False
    img_file_choiced = open('temp1.txt', 'w')
    count = 0
    for line in lines :
        if (line.find("begin 32x32")>0 ) : # 1108 is numbers of chars per image 400x300 
            print("thumbnail begin 32x32 1108")
            print(line)
            read_line = True
        elif (line.find("thumbnail e")>0 and read_line == True) : 
            print("thumbnail end")
            print(line)
            read_line = False
            img_file_choiced.close()
            break
        else :
            if read_line == True :
                dec_tmp = line.replace("; ", "")
                img_file_choiced.write(dec_tmp)
                print(dec_tmp)
    img_file_choiced.close()
    file = open('temp1.txt', 'rb')
    encoded_data = file.read()
    file.close()
    #decode base64 string data
    decoded_data=base64.b64decode((encoded_data))
    img_file = open('small_preview.png', 'wb')
    img_file.write(decoded_data)
    img_file.close()
    os.remove('temp1.txt')
    return
    return

def thumbarnail_large(file_choiced):
    print(file_choiced)
    file_choiced1 = open(file_choiced, "r")
    lines = file_choiced1.readlines()
    x = len(lines)
    print("thumbarnail function")
    read_line = False
    img_file_choiced = open('temp.txt', 'w')
    count = 0
    '''
    while True :
        if count >= x :
            break
    '''
    for line in lines :
        if (line.find("begin 400x300")>0 ) : # 21284 is numbers of chars per image 400x300 
            print("thumbnail begin 400x300 21284")
            print(line)
            read_line = True
        elif (line.find("thumbnail e")>0 and read_line == True) : 
            print("thumbnail end")
            print(line)
            read_line = False
            img_file_choiced.close()
            break
        else :
            if read_line == True :
                dec_tmp = line.replace("; ", "")
                img_file_choiced.write(dec_tmp)
                print(dec_tmp)
    img_file_choiced.close()
    file = open('temp.txt', 'rb')
    encoded_data = file.read()
    file.close()
    #decode base64 string data
    decoded_data=base64.b64decode((encoded_data))
    img_file = open('preview.png', 'wb')
    img_file.write(decoded_data)
    img_file.close()
    os.remove("temp.txt")
    return

def uid_bobine():
    pass
    
def nfctag_data():
    pass

def data_from_gcode():
    pass
    
if __name__ == "__main__":
    print("this is a library")
