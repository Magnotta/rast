from datetime import datetime

azimNums = []
elevNums = []
days = range(4,365,7)

with open("../dados/Tabela_azimute.txt",'r') as f:
    for line in f:
        cnums = line.split(',')
        azimNums.append([float(num) for num in cnums])

with open("../dados/Tabela_elevacao.txt",'r') as f:
    for line in f:
        cnums = line.split(',')
        elevNums.append([float(num) for num in cnums])

doy = datetime.now().timetuple().tm_yday
trackDoy = min(days, key=lambda x:abs(x-doy))
print(f"Current doy:{doy}\nDoy in list:{trackDoy}")
trackAzims = azimNums[days.index(trackDoy)]
trackElevs = elevNums[days.index(trackDoy)]
with open("../dados/raw.txt",'w') as f:
    f.write("const double trackAzims[] = {")
    for num in trackAzims:
        f.write(str(num))
        if(num):
            f.write(',')
        else:
            f.write('};\n')
            break

    f.write("const double trackElevs[] = {")
    for num in trackElevs:
        f.write(str(num))
        if(num):
            f.write(',')
        else:
            f.write('};\n')
            break