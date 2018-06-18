import uhal
import argparse
import datetime, time


if __name__ == "__main__":

    parser = argparse.ArgumentParser("")
    parser.add_argument('path',   help='The path to connection file .xml')
    parser.add_argument('id',     help='The id of the IOC as written in the connection file')

    args = parser.parse_args()

    manager = uhal.ConnectionManager("file://" + args.path)
    hw = manager.getDevice(args.id)


    #memport.getNode(FMC_CTRL_REG).write(0x00); hw.dispatch()
    reg = hw.getNode("sl1").getNode("FMC_CTRL_REG").read(); hw.dispatch()
    print "reg = ", reg

    
    print ("++++++++++++++++++++++++++++++++++++++++++++++\n")


