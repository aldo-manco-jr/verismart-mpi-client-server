#include <iostream>
#include <mpi.h>
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"
#include "rapidjson/include/rapidjson/filereadstream.h"
#include "rapidjson/include/rapidjson/filewritestream.h"
#include "rapidjson/include/rapidjson/writer.h"
#include <cstdio>
#include <vector>

using namespace rapidjson;

int main(int argc, char **argv) {

    std::cout << "Hello, from Client!" << std::endl;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    int initial_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &initial_rank);
    int initial_size;
    MPI_Comm_size(MPI_COMM_WORLD, &initial_size);

    MPI_Info info;
    MPI_Info_create(&info);
    MPI_Info_set(info, "ompi_global_scope", "true");

    char portNameOmpiServer[MPI_MAX_PORT_NAME];

    MPI_Comm interCommunicator;
    if (initial_rank == 0) {
        std::cout << "Looking up name!" << std::endl;
        MPI_Lookup_name("compute", info, portNameOmpiServer);
        std::cout << "Name lookup done: " << portNameOmpiServer << std::endl;
    }

    MPI_Comm_connect(portNameOmpiServer, info, 0, MPI_COMM_WORLD, &interCommunicator);
    std::cout << "Comm connected!" << std::endl;

    //float tmp=20;
    char tmp[3];
    tmp[0]='a';
    tmp[1]='b';
    tmp[2]='c';

//    for (int i = 0; i < 10; ++i) {
//        MPI_Send(&tmp, 1, MPI_DOUBLE, 0, 2, interCommunicator);
//        tmp-=0.1;
//    }

    MPI_Send(tmp, 3, MPI_CHAR, 0, 0, interCommunicator);

//    FILE* fp = fopen("file_tmp.json", "r+b"); // non-Windows use "r"
//
//    char writeBuffer[65536];
//
//    MPI_Recv(&writeBuffer, 1000, MPI_CHAR, 0, 0, interCommunicator, &status);
//
//    Document d;
//
//    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
//    Writer<FileWriteStream> writer(os);
//
//    d.Accept(writer);
//
//    FileReadStream is(fp, writeBuffer, sizeof(writeBuffer));
//    d.ParseStream(is);
//
//    fclose(fp);
//
//    std::cout << writeBuffer << std::endl;
//    fclose(fp);

//    std::cout << writeBuffer << std::endl;

    // string

//    char stringbuffer1[65536];
//    MPI_Recv(&stringbuffer1, 1000, MPI_CHAR, 0, 0, interCommunicator, &status);
//
//    Document d;
//    d.Parse(stringbuffer1);
//
//    // 2. Modify it by DOM.
//    Value& s = d["s0"];
//    s.SetInt(s.GetInt() + 1);
//
//    // 3. Stringify the DOM
//    StringBuffer buffer;
//    Writer<StringBuffer> writer(buffer);
//    d.Accept(writer);
//
//    // Output {"project":"rapidjson","stars":11}
//    std::cout << buffer.GetString() << std::endl;

    char stringbuffer1[65536];
    MPI_Recv(&stringbuffer1, 1000, MPI_CHAR, 0, 0, interCommunicator, &status);

    Document d;
    d.Parse(stringbuffer1);

    for (Value::ConstMemberIterator itr = d["s0"].MemberBegin(); itr != d["s0"].MemberEnd(); ++itr){
        std::cout << "Member: " << itr->name.GetString() << std::endl;

        Value& s = d["s0"][itr->name.GetString()];
        std::vector<std::vector<int>> vec;
        vec.resize(s.Size());

        for (SizeType i = 0; i<s.Size(); i++){
            const rapidjson::Value &data_vec = s[i];
            for (SizeType j = 0; j < data_vec.Size(); j++){
                vec[i].push_back(data_vec[j].GetInt()+1);
                std::cout << data_vec[j].GetInt() << std::endl;
            }
        }
        vec.clear();
    }

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
    //std::cout << vec[0][0] << std::endl;

    MPI_Comm newCommunicator;
    MPI_Intercomm_merge(interCommunicator, 1, &newCommunicator);

    int new_rank;
    MPI_Comm_rank(newCommunicator, &new_rank);
    int new_size;
    MPI_Comm_size(newCommunicator, &new_size);

    std::cout << "Old rank: " << initial_rank << ", new rank: " << new_rank << std::endl;
    std::cout << "Old size: " << initial_size << ", new size: " << new_size << std::endl;

    std::cout << "Disconnecting comm!" << std::endl;
    MPI_Comm_disconnect(&interCommunicator);
    std::cout << "Comm disconnected!" << std::endl;

    MPI_Finalize();
    return 0;
}