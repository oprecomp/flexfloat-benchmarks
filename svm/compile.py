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

    result.append(" -DFRAC_SVM_KERNEL_SUM=%d" % (config_vals[0] - 1))
    result.append(" -DFRAC_SVM_KERNEL_D=%d" % (config_vals[1] - 1))
    result.append(" -DFRAC_SVM_KERNEL_X=%d" % (config_vals[2] - 1))
    result.append(" -DFRAC_SVM_KERNEL_Y=%d" % (config_vals[3] - 1))
    result.append(" -DFRAC_SVM_KERNEL_TEMP1=%d" % (config_vals[4] - 1))
    result.append(" -DFRAC_SVM_PREDICT_KVALUE=%d" % (config_vals[5] - 1))

    result.append(" -DEXP_SVM_KERNEL_SUM=%d" % set_coefficient_bits(config_vals[0]))
    result.append(" -DEXP_SVM_KERNEL_D=%d" % set_coefficient_bits(config_vals[1]))
    result.append(" -DEXP_SVM_KERNEL_X=%d" % set_coefficient_bits(config_vals[2]))
    result.append(" -DEXP_SVM_KERNEL_Y=%d" % set_coefficient_bits(config_vals[3]))
    result.append(" -DEXP_SVM_KERNEL_TEMP1=%d" % set_coefficient_bits(config_vals[4]))
    result.append(" -DEXP_SVM_PREDICT_KVALUE=%d" % set_coefficient_bits(config_vals[5]))


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
