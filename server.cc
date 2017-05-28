
/* Server */

#include <iostream>
#include <memory>
#include <string>
#include <grpc++/grpc++.h>
#include "server_client.grpc.pb.h"
#include <glog/logging.h>


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using server_client::Request;
using server_client::JobDescriptor;
using server_client::TaskDescriptor;
using server_client::Task;


class TaskService final : public Task::Service {
  Status Send(ServerContext* context, const Request* request,
                  JobDescriptor* jd) override {
    LOG(INFO) << "Server process notified about call...." << std::endl;
    LOG(INFO) << "Server started processing client request...." << std::endl;
    jd->set_uuid("1234");
    jd->set_name(request->str_request() + "_job");
    jd->set_state(JobDescriptor::CREATED);
    TaskDescriptor* root_td = jd->mutable_root_task();
    root_td->set_uid(1);
    root_td->set_name(request->str_request() + "_task");
    root_td->set_state(TaskDescriptor::CREATED);
    root_td->set_job_id(jd->uuid());    
    return Status::OK;
  }
};


void RunServer() {
  std::string server_address("0.0.0.0:50051");
  TaskService service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  LOG(INFO) << "Server process started listening...." << std::endl;
  std::cout << "Server listening.. " << server_address << std::endl;
  server->Wait();
}


int main(int argc, char** argv) 
{
  google::InitGoogleLogging(argv[0]);
  LOG(INFO) << "Server process started running...." << std::endl;
  RunServer();

  return 0;
}
