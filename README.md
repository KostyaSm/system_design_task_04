
# Фитнес-трекер API

Сервис для управления пользователями, упражнениями и тренировками. Реализован на C++20 с использованием фреймворка userver и базы данных MongoDB.

## Технологический стек
- **Язык:** C++20
- **Фреймворк:** userver 
- **База данных:** MongoDB 7
- **Контейнеризация:** Docker / Docker Compose
- **Сборка:** CMake 3.16+

## Запуск проекта

```bash
docker compose up --build
```

Сервис будет доступен по адресам:
- **API:** http://localhost:8080
- **MongoDB:** mongodb://localhost:27017/fitness_tracker

Тестовый пароль для всех пользователей: `test123`

## API Endpoints

### Auth
- `POST /api/v1/auth/register` — регистрация
- `POST /api/v1/auth/login` — вход, получение токена

### Users
- `GET /api/v1/users/by-login?login=...` — поиск по логину
- `GET /api/v1/users/search?mask=...` — поиск по маске имени/фамилии

### Exercises
- `POST /api/v1/exercises` — создание упражнения (требует авторизации)
- `GET /api/v1/exercises?category=...&difficulty=...` — список упражнений

### Workouts
- `POST /api/v1/workouts` — создание тренировки
- `POST /api/v1/workouts/{id}/exercises` — добавление упражнения в тренировку
- `GET /api/v1/users/me/workouts?limit=20&offset=0` — история тренировок
- `GET /api/v1/workouts/statistics?from=YYYY-MM-DD&to=YYYY-MM-DD` — статистика

## Документная модель MongoDB
описание структуры коллекций и обоснование выбора Embedded/References в `schema_design.md`.

### Основные коллекции:
- `users` - данные пользователей
- `exercises` - библиотека упражнений
- `workouts` - тренировки с встроенным массивом выполненных упражнений
- `counters` - техническая коллекция для генерации числовых ID

## Тестирование
Интеграционные тесты на Python
```bash
pytest tests/
```
