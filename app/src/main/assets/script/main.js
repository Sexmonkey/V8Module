//mylib.var = mylib.PI + mylib.fun();
//var x = new mylib.X(1, true);
//mylib.prop = x.prop +mylib.fun();
var a = require("a");
var b = require("b");
var a = require("a");
NativeDevice.print(b.PI);
NativeDevice.print(a.person.name,a.person.age);
var person = new NativeDevice.Person("xiao yu",18);
NativeDevice.print(person.name,person.age);
person.setName("xiao xue");
person.setAge(19);
NativeDevice.print(person.name,person.age);
var y = NativeDevice.add(20, 30);
var z = 100;
NativeDevice.print(y);

