import cv2
import threading
import time
import argparse
from queue import LifoQueue, Queue
import logging
import os
import sys


class Sensor:
    def get(self):
        raise NotImplementedError("subclasses must implement method get()")


class SensorX(Sensor):
    """Sensor X"""

    def __init__(self, delay: float):
        self.delay = delay
        self.data = 0

    def get(self) -> int:
        time.sleep(self.delay)
        self.data += 1
        return self.data


class SensorCam(Sensor):
    def __init__(self, camera_index, width, height, glb_event):
        '''Конструктор __init__ пытается открыть камеру с заданным camera_index.
        Если камера не открывается, логируется ошибка и устанавливается флаг glb_event, вызывается метод stop.
        Если камера открыта, устанавливаются параметры ширины и высоты.'''
        try:
            self._cap = cv2.VideoCapture(camera_index)
            if not self._cap.isOpened():
                logging.error(
                    "We don't have a camera with this camera index: %d", 
                    camera_index
                )
                glb_event.set()
                self.stop()
            else:
                self._cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
                self._cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
        except Exception as e:
            logging.error(
                "We have an error: %s", str(e)
                )
            glb_event.set()
            self.stop()
            raise

    def get(self):
        '''Метод get считывает кадр с камеры и возвращает его. Если кадр не был получен, логируется ошибка и останавливается камера'''
        ret, frame = self._cap.read()
        if not ret:
            logging.error(
                "we have no img from cam"
                )
            glb_event.set()
            self.stop()
            return None
        return frame

    def stop(self):
        '''Метод stop освобождает ресурсы камеры'''
        self._cap.release()

    def __del__(self):
        '''Метод __del__ вызывается при удалении объекта и освобождает ресурсы камеры'''
        self.stop()


def sensor_worker(sensor: SensorX, queue: LifoQueue, event_funk):
    '''Функция sensor_worker запускается в отдельном потоке для работы с сенсором SensorX.
    Пока установлен флаг event_funk, функция получает данные с сенсора и помещает их в очередь.
    Если очередь полна, удаляется старый элемент.
    В конце, когда флаг event_funk сбрасывается, выводится сообщение о завершении работы'''
    while event_funk.is_set():
        a = sensor.get()
        if queue.full():
            queue.get()
        queue.put(a)
    print("!!! Sensor worker done, hurray !!!")


def camera_worker(queue: Queue, args, glb_event, event_funk):
    '''Функция camera_worker запускается в отдельном потоке для работы с камерой.
    Создается объект SensorCam.
    Пока установлен флаг event_funk, функция получает кадры с камеры и помещает их в очередь.
    Если кадр не был получен, цикл прерывается.
    Если очередь полна, удаляется старый элемент.
    В конце вызывается метод stop для освобождения ресурсов камеры'''
    cam = SensorCam(args.camIndex, args.size[0], args.size[1], glb_event)
    while event_funk.is_set():
        a = cam.get()
        if a is None:
            break
        if queue.full():
            queue.get()
        queue.put(a)
    cam.stop()


class ImageWindow:
    def __init__(self, fps, height):
        '''Конструктор __init__ инициализирует данные сенсоров как [0, 0, 0], устанавливает частоту кадров и высоту изображения'''
        self._sensor_data = [0, 0, 0]
        self.frame = None
        self.fps = fps
        self._height = height

    def show(self, cam_queue: Queue, queues: list):
        '''Метод show отображает данные:
        Извлекает данные из очередей сенсоров и обновляет их.
        Если очередь камеры не пуста, извлекается кадр и на него накладывается текст с данными сенсоров.
        Кадр отображается в окне'''
        try:
            for i in range(3):
                if not queues[i].empty():
                    self._sensor_data[i] = queues[i].get()
                    print(self._sensor_data[i])
            if not cam_queue.empty():
                self.frame = cam_queue.get()
                cv2.putText(
                    self.frame,
                    f"Sensor1: {self._sensor_data[0]}  Sensor2: {self._sensor_data[1]}  Sensor3: {self._sensor_data[2]}",
                    (10, self._height - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 255), 1,
                )
                cv2.imshow("camera and data", self.frame)
        except Exception as e:
            logging.error(
                "We have an error at show(): %s", str(e)
                )

    def stop(self):
        '''Метод stop закрывает все окна OpenCV'''
        cv2.destroyAllWindows()


if __name__ == "__main__":
    #Создание парсера аргументов командной строки.
    parser = argparse.ArgumentParser(description="width, height, fps")

    parser.add_argument("--camIndex", 
                        type=int, 
                        default=0)
    
    parser.add_argument("--size", 
                        nargs=2, 
                        type=int, 
                        default=[720, 480], 
                        help="Size as 'width height'")
    
    parser.add_argument("--fps", 
                        type=int,
                        default=60)
    
    args = parser.parse_args()

    #Проверка существования директории для логов, создание её при отсутствии.
    #Настройка логирования: файл для логов, уровень логирования (ERROR), формат сообщений
    if not os.path.exists("log"):
        os.makedirs("log")
    log_file = os.path.join("log", "error.log")
    logging.basicConfig(filename=log_file, level=logging.ERROR, format="%(asctime)s - %(levelname)s - %(message)s")

    #Создание событий для управления потоками. glb_event используется для глобальных ошибок, event_funk — для управления работой потоков
    glb_event = threading.Event()
    event_funk = threading.Event()
    
    glb_event.clear()
    event_funk.set()

    #Создание трех объектов SensorX с различными задержками
    #Создание очередей для данных сенсоров и камеры
    sensors = [SensorX(i) for i in [0.01, 0.1, 1]]
    sensor_queues = [LifoQueue(maxsize=1) for _ in range(3)]
    cam_queue = Queue(maxsize=1)

    #Создание потоков для работы с сенсорами и камерой
    sensor_workers = [
        threading.Thread(target=sensor_worker, args=(sensors[i], sensor_queues[i], event_funk))
        for i in range(3)
    ]
    cam_worker = threading.Thread(target=camera_worker, args=(cam_queue, args, glb_event, event_funk))
    
    #Создание объекта ImageWindow для отображения данных
    time.sleep(1)
    window_imager = ImageWindow(fps=args.fps, height=args.size[1])

    #Запуск потоков сенсоров и камеры
    for worker in sensor_workers:
        worker.start()
    cam_worker.start()

    #Отображение данных и проверка условий для остановки программы
    while True:
        window_imager.show(cam_queue, sensor_queues)

        if ((cv2.waitKey(1) & 0xFF == ord("q")) 
            or glb_event.is_set()):
            window_imager.stop()
            event_funk.clear()
            cam_worker.join()
            
            for worker in sensor_workers:
                worker.join()
            break
        
        time.sleep(1 / window_imager.fps)