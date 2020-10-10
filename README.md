# 0.Instruction
This Repos contains how to run yolov5 model in DeepStream 5.0

# 1.Geneate yolov5 engine model 
We can use https://github.com/wang-xinyu/tensorrtx yolov5 to generate engine model

### Important Note:
   You should replace yololayer.cu and hardswish.cu file in tensorrtx/yolov5

### How to Run, yolov5s as example
-- a). generate yolov5s.wts from pytorch with yolov5s.pt
```
git clone https://github.com/wang-xinyu/tensorrtx.git
git clone https://github.com/ultralytics/yolov5.git
// download its weights 'yolov5s.pt'
// copy tensorrtx/yolov5/gen_wts.py into ultralytics/yolov5
// ensure the file name is yolov5s.pt and yolov5s.wts in gen_wts.py
// go to ultralytics/yolov5
python gen_wts.py
// a file 'yolov5s.wts' will be generated.
```
-- b). build tensorrtx/yolov5 and run
```
// put yolov5s.wts into tensorrtx/yolov5
// go to tensorrtx/yolov5
// ensure the macro NET in yolov5.cpp is s
mkdir build
cd build
cmake ..
make
sudo ./yolov5 -s             // serialize model to plan file i.e. 'yolov5s.engine'
```
We can get 'yolov5s.engine' and 'libmyplugin.so' here for the future use.

# 2.Build DeepStream 5.0 nvdsinfer_custom_impl_yolo plugin
In Deepstream 5.0/nvdsinfer_custom_impl_Yolo Directory, exec 'make' command.

We can get libnvdsinfer_custom_impl_Yolo.so here.


# 3.Modify configure file
After build yolov5 plugin, modify 'config_infer_primary_yoloV5.txt' in Deepstream 5.0 Directory.

-- a).In Line 58. "parse-bbox-func-name=NvDsInferParseCustomYoloV5"   // This is the bbox parse function name.

-- b).In Line 59. "custom-lib-path"   // This is DeepStream plugin path.

-- c).In Line 56. Comment "#cluster-mode=2". Becase we use custom NMS function.

# 4. How to run it

Running the application as
```
LD_PRELOAD=./libcustomOp.so deepstream-app -c <app-config>
```