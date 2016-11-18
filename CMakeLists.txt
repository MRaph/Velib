# Ce fichier est lu par la commande cmake
# http://www.cmake.org/cmake/help/documentation.html
#
# Cette commande non obligatoire est fortement conseillée.
cmake_minimum_required (VERSION 2.8)

# Déclaration du projet.
# Les fichiers CMakeLists pourront référer à la racine du projet par la variable
# ${VELIB_SOURCE_DIR} et ${VELIB__BINARY_DIR}
project(VELIB_)
set(EXE run)

# L'exécutable sera rangé différemment en fonction de la plateformee.
# Par défaut le binaire est construit dans le répertoire courant (?)
# set(EXECUTABLE_OUTPUT_PATH ../bin)
# set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE} )

# cas particuier pour windoze (sans cygwin) : non testé
# IF(WIN32 AND NOT UNIX)
#     SET(CPACK_NSIS_MODIFY_PATH ON)
# ENDIF(WIN32 AND NOT UNIX)



# Variables de configution pour cmake
list(APPEND CMAKE_CXX_FLAGS ${CPLEX_CXX_FLAGS}) # on ajoute au lieu d'affecter
list(APPEND CMAKE_EXE_LINKER_FLAGS ${CPLEX_CXX_LDFLAGS}) # on ajoute au lieu d'affecter
list(APPEND CMAKE_CXX_FLAGS_DEBUG "-g -O0") # on ajoute au lieu d'affecter
list(APPEND CMAKE_CXX_FLAGS_RELEASE "-O3 -Werror") # on ajoute au lieu d'affecter

# Les options de compilation supplémentaires (valable en C et en C++)
# add_definitions(-Wall -Wextra -ansi -O2 -Wwrite-strings
#                 -Wuninitialized -Wno-missing-braces
#                 -Wno-missing-field-initializers)
# EN PARTIE REDONDANT AVEC CMAKE_CXX_FLAGS !
add_definitions(
    -Wall
    -Wno-unused-private-field  # inhiber un warning pour champs privé inutilisé
    -Wno-sign-compare # inhiber warning de comparaison entre int et xxx.size()
    -m64
    -fPIC
    -fexceptions
    -std=c++11
    -pg # Generate extra code for profile info (e.g.  gprof)
    -DNDEBUG
    -DIL_STD  # Pour cplex en C++
    -O
    -g
    # -O2
    # -O3
)


# On définit la liste des tous les fichiers sources (dont les includes)
# cmake se débrouillera pour trouver le main() et les dépendances.
# On peut préciser des sous-répertoires, des groupe de fichiers, ...
# MAIS ATTENTION au fichiers sources parasites (inutilisés) qui traineraient
# même non utilisees, ils risquent de provoquer des erreurs de compilation !
file(
    GLOB
    srcs
    src/*.cpp
    src/*.cc
    src/*.c
    # src/*.hpp
    # src/*.hh
    # src/*.h
    *.cpp # SPECIAL POUR EXEMPLE MONO_FICHIER cutstock.cpp
)

# Les répertoires ou chercher vos includes (ou ceux de cplex, ...)
include_directories(
  # ${PROJECT_SOURCE_DIR}
  # ${PROJECT_BINARY_DIR}
  # ${ILOG_CONCERT_INCLUDE_DIR}
  # ${ILOG_CPLEX_INCLUDE_DIR}
  # $ENV{LEMON_INCLUDE}
  # $ENV{GLPK_INCLUDE}
)

# Ajouter ici les répertoire de vos bib. dynamique  utilisée
#
link_directories(
  # ${PROJECT_BINARY_DIR}/lemon
  # $ENV{LEMON_LIB}
  # $ENV{GLPK_LIB}
  # ${ILOG_CONCERT_LIB_DIR}
  # ${ILOG_CPLEX_LIB_DIR}
)

# On définit les exécutables ou bibliothèques à générer
# Il y a aussi add_library(...) pour créer une bibliothèques
# Et on peut utiliser add_dependencies(execB libB) pour gérer les
# inter-dépendances
add_executable(${EXE} ${srcs})

# Ajouter ici les bibliothèques dynamiques que vous utilisez
#
target_link_libraries(${EXE} m)        # lib mathématique
target_link_libraries(${EXE} pthread)  # pour programmation multithead
# target_link_libraries(${EXE} ilocplex) # spécial cplex
# target_link_libraries(${EXE} cplex)    # spécial cplex
# target_link_libraries(${EXE} concert)  # spécial cplex
# target_link_libraries(${EXE} emon)   # lib de graphe
# target_link_libraries(${EXE} glpk)   # solveur PLNE gratuit

# Quelques messages précisant la configuration utilisée
#
# message("=> ILOG_CPLEX_INCLUDE_DIR     is   ${ILOG_CPLEX_INCLUDE_DIR}")
# message("=> ILOG_CPLEX_LIB_DIR         is   ${ILOG_CPLEX_LIB_DIR}")
# message("=> ILOG_CONCERT_INCLUDE_DIR   is   ${ILOG_CONCERT_INCLUDE_DIR}")
# message("=> ILOG_CONCERT_LIB_DIR       is   ${ILOG_CONCERT_LIB_DIR}")
# message("=> LEMON_INCLUDE              is   $ENV{LEMON_INCLUDE}")
# message("=> LEMON_LIB                  is   $ENV{LEMON_LIB}")
# message("=> GLPK_INCLUDE               is   $ENV{GLPK_INCLUDE}")
# message("=> GLPK_LIB                   is   $ENV{GLPK_LIB}")
message("=> srcs            is   ${srcs}")
# Ceci affiche 8 sur un machine 64 bits ou 4 sur un machine 32 bit
message("=> CMAKE_SIZEOF_VOID_P    is   ${CMAKE_SIZEOF_VOID_P}")
message("\n")



#
# Complément pour ajouter quelques cibles personnalisées dans le Makefile généré
#
EXECUTE_PROCESS(
    COMMAND date +%Y%m%d-%Hh%M
    OUTPUT_VARIABLE date_stamp
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
GET_FILENAME_COMPONENT( project_dir_name ${CMAKE_SOURCE_DIR} NAME )


# On peut compléter les cible du Makefile généré
# (la comande finale de cette ciblesera ajoutée à CMakeFiles/Makefile2)
# (puis Essayer de créer un cmake tbz !!)
ADD_CUSTOM_TARGET(distclean
    COMMAND @echo Nettoyage complet des sources
    COMMAND @echo \"Répertoire courant = `pwd`\"
    COMMAND @echo "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}"
    # COMMAND make clean
    COMMAND find ${CMAKE_CURRENT_BINARY_DIR} -name "CMakeCache.txt" | xargs rm -rf
    COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/*
)
ADD_CUSTOM_TARGET(dc
    COMMAND make distclean
)
ADD_CUSTOM_TARGET(cc
    COMMAND make distclean
)
# Attention : cette commande construit une cible pour le Makefile.
# Il faut protéger les double-quote si l'on veux qu'elles ne soient pas consommées
# par cmake mais passée au Makefile.
# Un seul COMMENT par cible semble-t-il
ADD_CUSTOM_TARGET(tbz
    COMMENT "Création d'une archive datée du projet"
    COMMAND @echo \" => duplication du projet en : ${project_dir_name}-${date_stamp}\"
    COMMAND cp -Rp ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}-${date_stamp}
    COMMAND rm -r ${CMAKE_SOURCE_DIR}-${date_stamp}/build/
    COMMAND mkdir -p ${CMAKE_SOURCE_DIR}-${date_stamp}/build/
    # tar -C newDir :  pour se placer dans le répertoire parent de l'archive
    COMMAND tar cjf  ${CMAKE_SOURCE_DIR}-${date_stamp}.tbz
                     -C ${CMAKE_SOURCE_DIR}/..
                     ${project_dir_name}-${date_stamp}
    COMMAND rm -r ${CMAKE_SOURCE_DIR}-${date_stamp}
    COMMAND @echo \" => Archive faite :  ${project_dir_name}-${date_stamp}.tbz\"
)
ADD_CUSTOM_TARGET(txz
    COMMENT "Création d'une archive datée du projet (TEST XZ)"
    COMMAND @echo \" => duplication du projet en : ${project_dir_name}-${date_stamp}\"
    COMMAND cp -Rp ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}-${date_stamp}
    COMMAND rm -r ${CMAKE_SOURCE_DIR}-${date_stamp}/build/
    COMMAND mkdir -p ${CMAKE_SOURCE_DIR}-${date_stamp}/build/
    # tar -C newDir :  pour se placer dans le répertoire parent de l'archive
    COMMAND tar cf  ${CMAKE_SOURCE_DIR}-${date_stamp}.tar
                     -C ${CMAKE_SOURCE_DIR}/..
                     ${project_dir_name}-${date_stamp}
    COMMAND xz  ${CMAKE_SOURCE_DIR}-${date_stamp}.tar
    COMMAND mv    ${CMAKE_SOURCE_DIR}-${date_stamp}.tar.xz ${CMAKE_SOURCE_DIR}-${date_stamp}.txz
    COMMAND rm -r ${CMAKE_SOURCE_DIR}-${date_stamp}
    COMMAND @echo \" => Archive faite :  ${project_dir_name}-${date_stamp}.tbz\"
)

INCLUDE(InstallRequiredSystemLibraries)
IF(WIN32 AND NOT UNIX)
    SET(CPACK_NSIS_MODIFY_PATH ON)
ENDIF(WIN32 AND NOT UNIX)

INCLUDE(CPack)

#./



