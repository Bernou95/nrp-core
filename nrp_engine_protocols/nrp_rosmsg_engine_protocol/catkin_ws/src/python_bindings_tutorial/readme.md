In order to test this package you can must use the following lines


>>> from std_msgs.msg import Int64
>>> from python_bindings_tutorial import AddTwoInts


Next lines are suggested


>>> from std_msgs.msg import Int64
>>> from python_bindings_tutorial import AddTwoInts
>>> a = Int64(4)
>>> b = Int64(2)
>>> addtwoints = AddTwoInts()
>>> sum = addtwoints.add(a,b)
>>> print(sum)
data: 6
>>> 


Make sure to compile using catkin_make on ~/catkin_ws/ before trying to use this package
