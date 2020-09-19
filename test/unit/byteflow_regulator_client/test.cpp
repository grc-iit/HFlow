//
// Created by neeraj on 9/18/20.
//

#include <rhea/client/ByteFlow_Regulator_Client.h>

int main(int argc, char * argv[]){
    MPI_Init(&argc,&argv);
    MPI_Barrier(MPI_COMM_WORLD);

    int ioMode, ioOperation, requestSize, requestNumber;
    std::string distributionMode;

    if(argc > 1){
        SENTINEL_CONF->CONFIGURATION_FILE=argv[1];
    }
    BASKET_CONF->BACKED_FILE_DIR=SENTINEL_CONF->BYTEFLOW_REGULATOR_DIR;
    auto client = rhea::ByteFlow_Regulator_Client();
    printf("Client Setup\n");
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Changing Conf\n");

    MPI_Barrier(MPI_COMM_WORLD);
    printf("Done Conf\n");

    client.SetOutRate(0, 111, 100);
    client.SetInRate(0, 111, 100);

    MPI_Finalize();
    return 0;
}
