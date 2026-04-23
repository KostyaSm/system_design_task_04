#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/mongodb/component.hpp>
#include <userver/utils/daemon_run.hpp>

#include "handlers/auth_handlers.hpp"
#include "handlers/user_handlers.hpp"
#include "handlers/exercise_handlers.hpp"
#include "handlers/workout_handlers.hpp"


#include "middlewares/auth_middleware.hpp"
#include "storage/fitness_storage.hpp"




int main(int argc, char* argv[]) {
    auto component_list = userver::components::MinimalServerComponentList()
        .Append<userver::storages::mongodb::Component>("mongodb")
        .Append<userver::server::handlers::Ping>()
        .Append<userver::server::handlers::TestsControl>()
        .Append<userver::clients::http::Component>()
        .Append<userver::clients::dns::Component>()
        .Append<storage::FitnessStorage>("fitness-storage")
        .Append<handlers::AuthRegisterHandler>("handler-auth-register")
        .Append<handlers::AuthLoginHandler>("handler-auth-login")
        .Append<handlers::UserByLoginHandler>("handler-user-by-login")
        .Append<handlers::UserSearchHandler>("handler-user-search")
        .Append<handlers::ExerciseCreateHandler>("handler-exercises-create")
        .Append<handlers::ExerciseListHandler>("handler-exercises-list")
        .Append<handlers::WorkoutCreateHandler>("handler-workouts-create")
        .Append<handlers::WorkoutAddExerciseHandler>("handler-workout-add-exercise")
        .Append<handlers::UserWorkoutsHandler>("handler-user-workouts")
        .Append<handlers::WorkoutsStatsHandler>("handler-workouts-stats")
        .Append<middlewares::AuthMiddleware>("auth-middleware");

    return userver::utils::DaemonMain(argc, argv, component_list);
}