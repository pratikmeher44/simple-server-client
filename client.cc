/* Client */

#include <iostream>
#include <memory>
#include <string>
#include <grpc++/grpc++.h>
#include "server_client.grpc.pb.h"
#include "server_job_desc.pb.h"
#include "server_task_desc.pb.h"
#include <glog/logging.h>


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using server_client::Request;
using server_client::JobDescriptor;
using server_client::Task;


class TaskClient {
 public:
    TaskClient(std::shared_ptr<Channel> channel): stub_(Task::NewStub(channel)) {}

    std::string Send(const std::string& str_request) {
    Request request;
    request.set_str_request(str_request);

    JobDescriptor jd;
    ClientContext context;

    LOG(INFO) << "Client stub sending request msg to server" << std::endl;
    Status status = stub_->Send(&context, request, &jd);

    if (status.ok()) {
      return jd.uuid();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<Task::Stub> stub_;
};

int main(int argc, char** argv)
{
  google::InitGoogleLogging(argv[0]);
  LOG(INFO) << "Client started execution..." << std::endl;

  // Create channel using IP and port of server
  TaskClient task(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  std::string request_str("Service Request");
  LOG(INFO) << "Sending request to server to get job descriptor" << std::endl;
  std::string response = task.Send(request_str);
  LOG(INFO) << "Client received: " << response << std::endl;
  std::cout << "Client received: " << response << std::endl;
  return 0;
}


