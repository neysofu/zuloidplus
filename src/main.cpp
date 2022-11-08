#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <engine.hpp>
#include <iostream>

int main(void) {
  std::cout << "# Welcome to ZuloidPlus.\n";
  std::cout << "# PID: "
            << boost::interprocess::ipcdetail::get_current_process_id()
            << std::endl;

  Engine engine = Engine();

  for (std::string line; std::getline(std::cin, line);) {
    auto status = engine.evaluate_command(line);
    if (status == Status::Quit) {
      break;
    }
  }

  return 0;
}
