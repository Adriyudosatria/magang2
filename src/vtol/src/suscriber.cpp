#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

using std::placeholders::_1;

class DroneSuscriber : public rclcpp::Node
{
public:
  DroneSuscriber()
      : Node("drone_suscriber")
  {
    subscription_ = this->create_subscription<std_msgs::msg::Int32>(
        "topic", 10, std::bind(&DroneSuscriber::topic_callback, this, _1));

  }

private:
  int event = 63;
  int mas_nf = 9;
  bool landed = false;

  void topic_callback(const std_msgs::msg::Int32 &msg)
  {
    int number = msg.data;

    if (cek(number))
    {
      event++;
      RCLCPP_INFO(this->get_logger(), "%d paliodrom, event = %d", number, event);
      if (event % 67 == 0)
      {
        mas_nf++;
        RCLCPP_INFO(this->get_logger(), " mas nf terdeteksi = %d", mas_nf);
      }
    }
    else
    {

      RCLCPP_INFO(this->get_logger(), "%d bukan paliodrom", number);
    }
    if (mas_nf == 10 && !landed)
    { 
      landed = true;
      RCLCPP_INFO(this->get_logger(), "landing");
    }
  }
  bool cek(int n) const
  {

    int awal = n;
    int akhir = 0;

    while (n > 0)
    {
      akhir = akhir * 10 + (n % 10);
      n /= 10;
    }
    return awal == akhir;
  }

  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_; 
  
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DroneSuscriber>());
  rclcpp::shutdown();
  return 0;
}