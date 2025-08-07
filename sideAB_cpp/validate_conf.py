#! /usr/bin/env python3
# YAML を json schema で validate する
# cf: https://qiita.com/ssc-ksaitou/items/c5f580c70bd9c7e76b6c

import os
import sys
import yaml
import jsonschema
import json
import glob
from jsonschema import RefResolver

schema_data="""{
  "$schema": "http://json-schema.org/schema#",
  "type": "object",
  "properties": {
    "meta": {
      "type": "object",
      "properties": {
        "update": {
          "type": "string"
        },
        "author": {
          "type": "string"
        }
      },
      "required": [
      ]
    },
    "sideA": {
      "type": "object",
      "properties": {
        "pub_addr": {
          "type": "string"
        },
        "sub_addr": {
          "type": "string"
        },
        "data_size": {
          "type": "integer"
        },
        "flood_size": {
          "type": "integer"
        },
        "interval_ms": {
          "type": "integer"
        },
        "loop_num": {
          "type": "integer"
        }
      },
      "required": [
        "data_size",
        "flood_size",
        "interval_ms",
        "loop_num",
        "pub_addr",
        "sub_addr"
      ]
    },
    "sideB": {
      "type": "object",
      "properties": {
        "sub_addr": {
          "type": "string"
        },
        "pub_addr": {
          "type": "string"
        }
      },
      "required": [
        "pub_addr",
        "sub_addr"
      ]
    }
  },
  "required": [
    "sideA",
    "sideB"
  ]
}
"""

def custom_tag_constructor(loader, node):
    return loader.construct_scalar(node)

# !vault などのYAMLタグのプロセッサを適当に定義する
#yaml.SafeLoader.add_constructor('!vault', custom_tag_constructor)

# YAML を JSON Schema で検証する
def validate_yaml(yaml_file_path):
    # JSON Schema のロード
    #with open(schema_file_path, "r") as schema_file:
    #    schema = json.load(schema_file)
    #    pass
    schema = json.loads(schema_data)
    
    # JSON Schema から外部ファイル参照があった場合にスキーマと同じディレクトリを探索するようにする
    #schema_directory = os.path.abspath(os.path.dirname(schema_file_path))
    #resolver = RefResolver(base_uri=f"file://{schema_directory}/", referrer=schema)
    
    # YAML のロード
    with open(yaml_file_path, "r") as yaml_file:
        yaml_data = yaml.safe_load(yaml_file)
        pass
    
    # 失敗した場合 jsonschema.exceptions.ValidationError がスローされる
    #jsonschema.validate(instance=yaml_data, schema=schema, resolver=resolver)
    jsonschema.validate(instance=yaml_data, schema=schema)
    return

if __name__=='__main__':
    validate_yaml(sys.argv[1])
