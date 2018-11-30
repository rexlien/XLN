namespace cpp XLN.Game.Thrift
namespace csharp XLN.Game.Common.Thrift


//typedef binary (cpp2.type = "folly::IOBuf") IOBuf
//typedef binary (cpp2.type = "std::unique_ptr<folly::IOBuf>") IOBufPtr

struct EntityMessage {

  1: i32 id,
  2: string name,
  3: map<string, string> properties
}


service EntityService {
  
  void sendMessage(1: EntityMessage msg);
}
