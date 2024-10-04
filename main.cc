#include <iostream>
#include <wayland-client-protocol-unstable.hpp>

using namespace wayland;
 
// Wayland protocol info dumper
class dumper
{
private:
  // global objects
  display_t display;
  registry_t registry;
 
public:
  dumper()
  {
  }
 
  ~dumper()
  {
  }
 
  void run()
  {
    std::vector<output_t> outputs;
    // retrieve global objects
    registry = display.get_registry();
    registry.on_global() = [&] (uint32_t name, std::string interface, uint32_t version)
      {
        std::cout << "* Global interface " << interface << " (name " << name << " version " << version << ")" << std::endl;
        if(interface == output_t::interface_name)
        {
          outputs.emplace_back();
          auto& output = outputs.back();
          registry.bind(name, output, version);
          output.on_geometry() = [=](int32_t x, int32_t y, int32_t physw, int32_t physh, output_subpixel subp, std::string make, std::string model, output_transform transform)
          {
            std::cout << "* Output geometry for " << output.get_id() << ":" << std::endl
              << "   Maker:   " << make << std::endl
              << "   Model:   " << model << std::endl
              << "   X:       " << x << std::endl
              << "   Y:       " << y << std::endl
              << "   PhysW:   " << physw << " mm" << std::endl
              << "   PhysH:   " << physh << " mm" << std::endl
              << "   Subpix:  " << static_cast<unsigned int>(subp) << std::endl
              << "   Transf:  " << static_cast<unsigned int>(transform) << std::endl;
          };
          output.on_scale() = [=](int32_t scale)
          {
            std::cout << "* Output scale for " << output.get_id() << ": " << scale << std::endl;
          };
          output.on_mode() = [=](uint32_t flags, int32_t width, int32_t height, int32_t refresh)
          {
            std::cout << "* Output mode for " << output.get_id() << ":" << std::endl
              << "   Width:   " << width << std::endl
              << "   Height:  " << height << std::endl
              << "   Refresh: " << refresh << " mHz" << std::endl
              << "   Flags:   " << flags << std::endl;
          };
        }
      };
    // Print global information
    display.roundtrip();
    std::cout << "------" << std::endl;
    // Print output information
    display.roundtrip();
  }
};
 
int main()
{
  dumper d;
  d.run();
  return 0;
}
