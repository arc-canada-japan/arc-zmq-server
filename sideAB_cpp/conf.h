//
// conf.h
//

#include <string>
#include <cstdio>
#include <yaml-cpp/yaml.h>

#ifndef TEST_WEBSOCKET_ECHOBACK_CONF_H_
#define TEST_WEBSOCKET_ECHOBACK_CONF_H_

namespace test_websocket_echoback {

class Conf {
 private:
  const std::string app_name_;
  const std::string yaml_path_;
  YAML::Node app_node_;

 public:
  Conf(const char *app_name, const char *yaml_path)
      : app_name_(app_name), yaml_path_(yaml_path),
        app_node_() {
    this->load();
  }
  virtual ~Conf() { }

 private:
  void load(void) {
    YAML::Node conf = YAML::LoadFile(yaml_path_);
    
    if (conf[app_name_]) {
      app_node_ = conf[app_name_];
      std::fprintf(stderr, "-- Conf::load(%s)\n", yaml_path_.c_str());
    } else {
      std::fprintf(stderr, "** Conf::load(%s): key '%s' not found.\n", yaml_path_.c_str(), app_name_.c_str());
    }
  }

 public:
  template <typename T> T get(const char *key) {
    if (! app_node_[key]) {
      std::fprintf(stderr, "** Conf(%s)[%s]: key '%s' not found.\n", yaml_path_.c_str(), app_name_.c_str(), key);
    }
    return app_node_[key].as<T>();
  }
  
  template <typename T> T get(const char *key) const {
    if (! app_node_[key]) {
      std::fprintf(stderr, "** Conf(%s)[%s]: key '%s' not found.\n", yaml_path_.c_str(), app_name_.c_str(), key);
    }
    return app_node_[key].as<T>();
  }
  
  std::string pub_addr(void) {
    const char *key = "pub_addr";
    if (! app_node_[key]) {
      std::fprintf(stderr, "** Conf::load(%s)[%s]: key '%s' not found.\n", yaml_path_.c_str(), app_name_.c_str(), key);
    }
    return app_node_[key].as<std::string>();
  }

  std::string sub_addr(void) {
    const char *key = "sub_addr";
    if (! app_node_[key]) {
      std::fprintf(stderr, "** Conf::load(%s)[%s]: key '%s' not found.\n", yaml_path_.c_str(), app_name_.c_str(), key);
    }
    return app_node_[key].as<std::string>();
  }
  

  int data_size(void) {
    const char *key = "data_size";
    if (! app_node_[key]) {
      std::fprintf(stderr, "** Conf::load(%s)[%s]: key '%s' not found.\n", yaml_path_.c_str(), app_name_.c_str(), key);
    }
    return app_node_[key].as<int>();
  }    

  int flood_size(void) {
    const char *key = "flood_size";
    if (! app_node_[key]) {
      std::fprintf(stderr, "** Conf::load(%s)[%s]: key '%s' not found.\n", yaml_path_.c_str(), app_name_.c_str(), key);
    }
    return app_node_[key].as<int>();
  }    

  int interval_ms(void) {
    const char *key = "interval_ms";
    if (! app_node_[key]) {
      std::fprintf(stderr, "** Conf::load(%s)[%s]: key '%s' not found.\n", yaml_path_.c_str(), app_name_.c_str(), key);
    }
    return app_node_[key].as<int>();
  }    
  
};

}

#endif  // TEST_WEBSOCKET_ECHOBACK_CONF_H_
