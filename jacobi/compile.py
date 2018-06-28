import sys
import csv
import subprocess

# Pulp with SmallFloat extensions
def set_coefficient_bits(prec):
    if(prec <= 3):                   # float8
        return 5
    elif(prec > 3 and prec <= 8):    # float16ext
        return 8
    elif(prec > 8 and prec <= 11):   # float16
        return 5
    elif(prec > 11 and prec <= 24):  # float32
        return 8
    elif(prec > 24 and prec <= 53):  # float64
        return 11
    else:
        raise Exception

def init_params(config_vals):
    result = []
    result.append(" -DFRAC_TMPNORM=%d" % (config_vals[0] - 1))
    result.append(" -DFRAC_BNORM=%d" % (config_vals[1] - 1))
    result.append(" -DFRAC_NORM=%d" % (config_vals[2] - 1))
    result.append(" -DFRAC_GRID=%d" % (config_vals[3] - 1))
    result.append(" -DFRAC_GRID_NEW=%d" % (config_vals[4] - 1))
    result.append(" -DFRAC_CONST1=%d" % (config_vals[5] - 1))
    result.append(" -DFRAC_CONST2=%d" % (config_vals[6] - 1))
    result.append(" -DFRAC_CONST3=%d" % (config_vals[7] - 1))
    result.append(" -DFRAC_TEMP1=%d" % (config_vals[8] - 1))
    result.append(" -DFRAC_TEMP2=%d" % (config_vals[9] - 1))
    result.append(" -DFRAC_TEMP3=%d" % (config_vals[10] - 1))
    result.append(" -DFRAC_TEMP4=%d" % (config_vals[11] - 1))
    result.append(" -DFRAC_TEMP5=%d" % (config_vals[12] - 1))
    result.append(" -DFRAC_TEMP6=%d" % (config_vals[13] - 1))
    result.append(" -DFRAC_TEMP7=%d" % (config_vals[14] - 1))
    result.append(" -DFRAC_TEMP8=%d" % (config_vals[15] - 1))
    result.append(" -DFRAC_TEMP9=%d" % (config_vals[16] - 1))
    result.append(" -DFRAC_TEMP10=%d" % (config_vals[17] - 1))
    result.append(" -DFRAC_TEMP11=%d" % (config_vals[18] - 1))
    result.append(" -DFRAC_TEMP12=%d" % (config_vals[19] - 1))
    result.append(" -DFRAC_TEMP13=%d" % (config_vals[20] - 1))
    result.append(" -DFRAC_TEMP14=%d" % (config_vals[21] - 1))
    result.append(" -DFRAC_TEMP15=%d" % (config_vals[22] - 1))
    result.append(" -DFRAC_TEMP16=%d" % (config_vals[23] - 1))
    result.append(" -DFRAC_TEMP17=%d" % (config_vals[24] - 1))

    result.append(" -DEXP_TMPNORM=%d" % set_coefficient_bits(config_vals[0]))
    result.append(" -DEXP_BNORM=%d" % set_coefficient_bits(config_vals[1]))
    result.append(" -DEXP_NORM=%d" % set_coefficient_bits(config_vals[2]))
    result.append(" -DEXP_GRID=%d" % set_coefficient_bits(config_vals[3]))
    result.append(" -DEXP_GRID_NEW=%d" % set_coefficient_bits(config_vals[4]))
    result.append(" -DEXP_CONST1=%d" % set_coefficient_bits(config_vals[5]))
    result.append(" -DEXP_CONST2=%d" % set_coefficient_bits(config_vals[6]))
    result.append(" -DEXP_CONST3=%d" % set_coefficient_bits(config_vals[7]))
    result.append(" -DEXP_TEMP1=%d" % set_coefficient_bits(config_vals[8]))
    result.append(" -DEXP_TEMP2=%d" % set_coefficient_bits(config_vals[9]))
    result.append(" -DEXP_TEMP3=%d" % set_coefficient_bits(config_vals[10]))
    result.append(" -DEXP_TEMP4=%d" % set_coefficient_bits(config_vals[11]))
    result.append(" -DEXP_TEMP5=%d" % set_coefficient_bits(config_vals[12]))
    result.append(" -DEXP_TEMP6=%d" % set_coefficient_bits(config_vals[13]))
    result.append(" -DEXP_TEMP7=%d" % set_coefficient_bits(config_vals[14]))
    result.append(" -DEXP_TEMP8=%d" % set_coefficient_bits(config_vals[15]))
    result.append(" -DEXP_TEMP9=%d" % set_coefficient_bits(config_vals[16]))
    result.append(" -DEXP_TEMP10=%d" % set_coefficient_bits(config_vals[17]))
    result.append(" -DEXP_TEMP11=%d" % set_coefficient_bits(config_vals[18]))
    result.append(" -DEXP_TEMP12=%d" % set_coefficient_bits(config_vals[19]))
    result.append(" -DEXP_TEMP13=%d" % set_coefficient_bits(config_vals[20]))
    result.append(" -DEXP_TEMP14=%d" % set_coefficient_bits(config_vals[21]))
    result.append(" -DEXP_TEMP15=%d" % set_coefficient_bits(config_vals[22]))
    result.append(" -DEXP_TEMP16=%d" % set_coefficient_bits(config_vals[23]))
    result.append(" -DEXP_TEMP17=%d" % set_coefficient_bits(config_vals[24]))


    return "".join(result)                                                                                                                                          

with open(sys.argv[1], 'r') as config_file:
    reader = csv.reader(config_file)
    row = next(reader)
    if row[-1] == '':
        del row[-1]
    config_vals = [int(x) for x in row]
    ext_cflags = init_params(config_vals)
    make_process = subprocess.Popen(
            "make clean all CONF_MODE=file EXT_CFLAGS=\"" + 
            ext_cflags + "\" OUTPUT_DIR=\"" + sys.argv[2] + "\" ", 
            shell=True, stderr=subprocess.STDOUT)
    make_process.wait()
