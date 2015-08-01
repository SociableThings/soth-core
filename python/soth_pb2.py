# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: soth.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='soth.proto',
  package='',
  syntax='proto2',
  serialized_pb=_b('\n\nsoth.proto\"\x8a\x01\n\rServoPosition\x12\x19\n\x11servo_front_right\x18\x01 \x02(\x05\x12\x18\n\x10servo_front_left\x18\x02 \x02(\x05\x12\x18\n\x10servo_back_right\x18\x03 \x02(\x05\x12\x17\n\x0fservo_back_left\x18\x04 \x02(\x05\x12\x11\n\tservo_yaw\x18\x05 \x02(\x05\"9\n\rLedBrightness\x12\x0b\n\x03red\x18\x01 \x02(\r\x12\r\n\x05green\x18\x02 \x02(\r\x12\x0c\n\x04\x62lue\x18\x03 \x02(\r\"F\n\x0bWeatherData\x12\x10\n\x08humidity\x18\x01 \x02(\x02\x12\x10\n\x08pressure\x18\x02 \x02(\x02\x12\x13\n\x0btemperature\x18\x03 \x02(\x02')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_SERVOPOSITION = _descriptor.Descriptor(
  name='ServoPosition',
  full_name='ServoPosition',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='servo_front_right', full_name='ServoPosition.servo_front_right', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='servo_front_left', full_name='ServoPosition.servo_front_left', index=1,
      number=2, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='servo_back_right', full_name='ServoPosition.servo_back_right', index=2,
      number=3, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='servo_back_left', full_name='ServoPosition.servo_back_left', index=3,
      number=4, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='servo_yaw', full_name='ServoPosition.servo_yaw', index=4,
      number=5, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=15,
  serialized_end=153,
)


_LEDBRIGHTNESS = _descriptor.Descriptor(
  name='LedBrightness',
  full_name='LedBrightness',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='red', full_name='LedBrightness.red', index=0,
      number=1, type=13, cpp_type=3, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='green', full_name='LedBrightness.green', index=1,
      number=2, type=13, cpp_type=3, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='blue', full_name='LedBrightness.blue', index=2,
      number=3, type=13, cpp_type=3, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=155,
  serialized_end=212,
)


_WEATHERDATA = _descriptor.Descriptor(
  name='WeatherData',
  full_name='WeatherData',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='humidity', full_name='WeatherData.humidity', index=0,
      number=1, type=2, cpp_type=6, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='pressure', full_name='WeatherData.pressure', index=1,
      number=2, type=2, cpp_type=6, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='temperature', full_name='WeatherData.temperature', index=2,
      number=3, type=2, cpp_type=6, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=214,
  serialized_end=284,
)

DESCRIPTOR.message_types_by_name['ServoPosition'] = _SERVOPOSITION
DESCRIPTOR.message_types_by_name['LedBrightness'] = _LEDBRIGHTNESS
DESCRIPTOR.message_types_by_name['WeatherData'] = _WEATHERDATA

ServoPosition = _reflection.GeneratedProtocolMessageType('ServoPosition', (_message.Message,), dict(
  DESCRIPTOR = _SERVOPOSITION,
  __module__ = 'soth_pb2'
  # @@protoc_insertion_point(class_scope:ServoPosition)
  ))
_sym_db.RegisterMessage(ServoPosition)

LedBrightness = _reflection.GeneratedProtocolMessageType('LedBrightness', (_message.Message,), dict(
  DESCRIPTOR = _LEDBRIGHTNESS,
  __module__ = 'soth_pb2'
  # @@protoc_insertion_point(class_scope:LedBrightness)
  ))
_sym_db.RegisterMessage(LedBrightness)

WeatherData = _reflection.GeneratedProtocolMessageType('WeatherData', (_message.Message,), dict(
  DESCRIPTOR = _WEATHERDATA,
  __module__ = 'soth_pb2'
  # @@protoc_insertion_point(class_scope:WeatherData)
  ))
_sym_db.RegisterMessage(WeatherData)


# @@protoc_insertion_point(module_scope)
