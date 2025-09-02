{ERROR} (20250902) running dbsk2d_ishock_esf_computation, which is Murathi's binary:
```
➜  shock git:(restruct) ✗ ./build/dbsk2d_ishock_esf_computation -x input_CH_test.xml
argv size: 3
[File]: Use input parameters: input_CH_test.xml
dborl_algo_params::parse_from_data() - could not find the node Sample_Shock
dborl_algo_params::parse_from_data() - could not find all params from the node Sample_Shock
```

{NON-BREAKING ERROR} (20250902) running dbsk2d_ishock_computation, which is an older build: 
```
➜  shock git:(restruct) ./build/dbsk2d_ishock_computation -x input_CH_test.xml 
argv size: 3
[File]: Use input parameters: input_CH_test.xml
test_data//0_colors.cemv loaded.
test_data//0_colors.png loaded.
************ Computing Shock *************
-400 -400
bbox (minx, miny) (maxx, maxy) (width, height): (-400, -400) (1200, 1200) (1600, 1600)
[Nverts=4 Area=2.56e+06 p0:(-400 -400)
 p1:(1200 -400)
 p2:(1200 1200)
 p3:(-400 1200)
]
Putting VSOL objects into boundary ... 
original polygon size: 4 fitted polygon size: 4
original polyline size: 744 fitted polyline size: 5
original polyline size: 15 fitted polyline size: 4
original polyline size: 5 fitted polyline size: 2
original polyline size: 4 fitted polyline size: 2
original polyline size: 69 fitted polyline size: 3
original polyline size: 1640 fitted polyline size: 16
done. Time taken= 0.001seconds
Partitioning boundary into cells...
Patitioning boundary...
Number of edges= 30
done. Time taken= 0seconds
Preprocessing boundary ... 
Analyzing boundary...
Number of contours= 7
Cell partition params:
Number of rows =1
Number of cols =1
Preprocess the boundary using partitioned cells: 
Cell row=0 column= 0  number of edges= 30
Preprocessing... done
Number of edges after preprocessing=30
done. Time taken= 0.002seconds
Verifying boundary preprocessing .. Boundary needs preprocessing No
Number of edges = 30
Number of belms = 95
summary: 

Put VSOL objects into boundary ......... 0.001 seconds.
Partition boundary into cells........... 0 seconds.
Preprocess boundary ....................  0.002 seconds.
Break long lines .......................  0 seconds.
Detecting the ishock from boundery ...
Detect shocks in each lagrangian cell
Start merging neighboring lagrangian cells until there is a single cell
shocks validated!
done. Time taken= 0.006seconds
Done with the update_shocks
************ IShock Output *************
Finish save .osf process!
[1]    364350 illegal hardware instruction (core dumped)  ./build/dbsk2d_ishock_computation -x input_CH_test.xml
``````
