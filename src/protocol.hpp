//Trying to be written in c or c++
typedef union Doublet{
  uint16_t integer;
  gf16 field;//PROBLEM this implementation doesn't exist in libff
} Doublet;
typedef struct PSP_header{
  uint8_t version;
  uint8_t reserved;
  Doublet packet_id;
  uint16_t message_id;
  uint16_t stream_id;
}
