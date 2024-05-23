from ultralytics import YOLO

model = YOLO('yolov8n-pose.pt')
res = model("input.mp4", save=True, device="cpu")
