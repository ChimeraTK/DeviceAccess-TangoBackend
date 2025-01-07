#!/usr/bin/python3

import argparse
import json

import tango


class Downloader:
    def __init__(self):
        parser = argparse.ArgumentParser()
        parser.add_argument('device')
        self.args = parser.parse_args()

    def run(self):
        device = tango.DeviceProxy(self.args.device)
        attributes = device.attribute_list_query_ex()
        registers = []

        for attribute in attributes:

            registers.append(
                {"name": attribute.label,
                 "length": attribute.max_dim_x,
                 "channels": attribute.max_dim_y,
                 "accessMode": self.deriveAccessMode(attribute),
                 "tangoTypeId": attribute.data_type,
                 "writable": attribute.writable})
        print(json.dumps({"catalogue": registers}, indent=4))

    def deriveAccessMode(self, attribute):
        return ""


if __name__ == "__main__":
    Downloader().run()
