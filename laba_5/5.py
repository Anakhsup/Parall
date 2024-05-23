import argparse
import threading
import queue
import time
import cv2
from ultralytics import YOLO

class VideoProcessor:
    def __init__(self, input_path, output_path, num_threads):
        '''
        Конструктор класса VideoProcessor.
        num_threads - количество потоков для предсказаний.
        frame_queue - очередь для хранения кадров видео.
        result_queue - очередь для хранения результатов предсказаний.
        stop_event - событие для остановки потоков.
        num_frames - получаем количество кадров в видео.
        fps - получаем количество кадров в секунду.
        '''
        self.input_path = input_path
        self.output_path = output_path
        self.num_threads = num_threads
        self.frame_queue = queue.Queue(1000)
        self.result_queue = queue.Queue()
        self.stop_event = threading.Event()

        cap = cv2.VideoCapture(input_path)
        self.num_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
        self.fps = cap.get(cv2.CAP_PROP_FPS)
        cap.release()

    def __enter__(self):
        '''
        Метод для инициализации потоков при входе в контекст.
        Создаем и запускаем потоки для чтения кадров, записи кадров и предсказаний.
        Запоминаем время начала работы.
        '''
        self.read_thread = threading.Thread(target=self.read_frames)
        self.write_thread = threading.Thread(target=self.write_frames)
        self.predict_threads = [threading.Thread(target=self.predict_and_enqueue) for _ in range(self.num_threads)]

        self.read_thread.start()
        self.write_thread.start()
        for thread in self.predict_threads:
            thread.start()

        self.start_time = time.monotonic()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        '''
        Метод для завершения работы потоков при выходе из контекста.
        Ожидание завершения всех потоков.
        Выводим время, затраченное на обработку.

        '''
        self.stop_event.set()

        for thread in self.predict_threads:
            thread.join()

        self.read_thread.join()
        self.write_thread.do_run = False
        self.write_thread.join()

        end_time = time.monotonic()
        print(f'Time taken: {end_time - self.start_time} seconds')

    def read_frames(self):
        '''
        Метод для чтения кадров из видеофайла.
        cap - открываем видеофалы.
        Читаем кадры и помещает их в frame_queue.
        '''
        cap = cv2.VideoCapture(self.input_path)
        frame_index = 0
        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                print("Error: Unable to read frame!")
                break
            self.frame_queue.put((frame, frame_index))
            frame_index += 1
            time.sleep(0.0001)
        self.stop_event.set()

    def write_frames(self):
        '''
        Метод для записи кадров в видеофайл.
        frames - массив для хранения всех кадров.
        Извлекаем кадры из result_queue и сохраняем их в массив.
        '''
        t = threading.current_thread()
        frames = [None] * self.num_frames
        while getattr(t, "do_run", True):
            try:
                frame, index = self.result_queue.get(timeout=1)
                frames[index] = frame
            except queue.Empty:
                pass
        print("Stopping frame writing.")

        print("Starting frame composition.")
        width, height = frames[0].shape[1], frames[0].shape[0]
        out = cv2.VideoWriter(self.output_path, cv2.VideoWriter_fourcc(*'mp4v'), self.fps, (width, height))

        for frame in frames:
            out.write(frame)
        out.release()

    def predict_and_enqueue(self):
        '''
        Метод для предсказаний на кадрах и помещения результатов в result_queue.
        Извлекаем кадры из frame_queue, делаем предсказания и сохраняем результаты в result_queue.
        '''
        local_model = YOLO(model="yolov8n-pose.pt", verbose=False)
        while True:
            try:
                frame, index = self.frame_queue.get(timeout=1)
                results = local_model.predict(source=frame, device='cpu')[0].plot()
                self.result_queue.put((results, index))
            except queue.Empty:
                if self.stop_event.is_set():
                    print(f'Thread {threading.get_ident()} exiting.')
                    break

def main(args):
    with VideoProcessor(args.input_video, args.output_video, args.num_threads) as processor:
        pass

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--input_video', type=str, default="input.mp4", help='Path to input video')
    parser.add_argument('--output_video', type=str, default="output.mp4", help='Path to output video')
    parser.add_argument('--num_threads', type=int, default=4, help='Number of threads')
    args = parser.parse_args()
    main(args)