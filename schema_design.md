# Проектирование документной модели: Фитнес-трекер

# Описание проекта
Приложение для учёта тренировок, упражнений и прогресса пользователей.  
Аналог MyFitnessPal с фокусом на силовые и кардио-тренировки.

# Сущности системы

| Сущность | Описание | Ключевые атрибуты |
|----------|----------|------------------|
| **User** | Пользователь приложения | login, name, email, settings |
| **Exercise** | Шаблон упражнения в библиотеке | name, category, muscle_groups, difficulty |
| **Workout** | Конкретная тренировка пользователя | date, duration, список выполненных упражнений |

---

# Структура коллекций MongoDB

### Коллекция `users`

```javascript
{
  _id: ObjectId,                    // внутренний ID MongoDB
  id: int,                          // числовой ID для API (генерируется через counters)
  login: string,                    // уникален, required, case-insensitive
  first_name: string,               // имя пользователя
  last_name: string,                // фамилия пользователя
  email: string | null,             // опционально, уникален если указан
  password_hash: string,            // хеш пароля (required)
  created_at: date,                 // дата регистрации
  updated_at: date,                 // дата последнего обновления профиля
  settings: {                       // embedded object: настройки пользователя
    unit_system: "metric" | "imperial",  // система единиц
    privacy: "public" | "private"        // видимость профиля
  },
  tokens: [                         // массив активных сессий
    {
      token: string,                // JWT или session token
      created_at: date,
      expires_at: date
    }
  ]
}
```

**Индексы:**
```javascript
db.users.createIndex({ login: 1 }, { unique: true, collation: { locale: "ru", strength: 2 } })
db.users.createIndex({ email: 1 }, { unique: true, sparse: true })
db.users.createIndex({ first_name_lc: 1, last_name_lc: 1 })  // для поиска по маске
```

---

### Коллекция `exercises`

```javascript
{
  _id: ObjectId,
  id: int,                          // числовой ID для API
  name: string,                     // уникальное название, required
  description: string | null,       // подробное описание техники
  category: "strength" | "cardio" | "flexibility" | "hiit",  // тип упражнения
  muscle_groups: [string],          // ["chest", "triceps", "shoulders"]
  difficulty: "beginner" | "intermediate" | "advanced",
  created_by: int,                  // reference → users.id (автор упражнения)
  created_at: date,
  is_public: boolean                // доступно всем или только автору
}
```

**Индексы:**
```javascript
db.exercises.createIndex({ name: 1 }, { unique: true, collation: { locale: "ru", strength: 2 } })
db.exercises.createIndex({ category: 1, difficulty: 1 })
db.exercises.createIndex({ created_by: 1 })
db.exercises.createIndex({ muscle_groups: 1 })  // для поиска по целевым мышцам
```

---

### Коллекция `workouts`

```javascript
{
  _id: ObjectId,
  id: int,                          // числовой ID для API
  user_id: int,                     // reference → users.id, required
  title: string,                    // название тренировки
  date: date,                       // дата и время начала, required
  duration_seconds: int | null,     // общая длительность в секундах
  notes: string | null,             // заметки пользователя
  exercises: [                      // EMBEDDED массив выполненных упражнений
    {
      exercise_id: int,             // reference → exercises.id
      exercise_name: string,        // денормализованное название (для истории)
      sets: int | null,             // количество подходов
      reps: int | null,             // повторений в подходе
      weight_kg: double | null,     // вес в кг (для strength)
      duration_seconds: int | null, // длительность подхода (для cardio)
      rest_seconds: int | null,     // отдых между подходами
      notes: string | null,         // заметки к подходу
      order: int                    // порядок выполнения в тренировке
    }
  ],
  created_at: date,
  updated_at: date
}
```

**Индексы:**
```javascript
db.workouts.createIndex({ user_id: 1, date: -1 })           // история тренировок по дате
db.workouts.createIndex({ user_id: 1, "exercises.exercise_id": 1 })  // статистика по упражнению
db.workouts.createIndex({ date: 1 })                         // для агрегаций по периоду
```

---

### Коллекция `counters` (техническая)

```javascript
{
  _id: "user_id" | "exercise_id" | "workout_id",  // тип счётчика
  seq: int                                         // текущее значение
}
```
