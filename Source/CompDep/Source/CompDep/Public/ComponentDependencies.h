#pragma once

#include "ComponentDependencyStructs.h"
#include "ComponentDependencies.generated.h"

/*
 * To implement dependencies in C++, form your class declaration like this:
class UMyClass : public UMyComponentParent, public IComponentDependencies
{
	GENERATED_BODY()
	COMPDEP_DECL()
...
 * And put this in your implementation file:

COMPDEP_IMPL_OVERRIDE_DEPS_BEGIN(ULuhorMovementComponent)
	COMPDEP_DEP_AnyOnActorRequired(UHittableComponent)
	COMPDEP_DEP_ChildWithTagOptional(UMeleeAttackerComponent, "my_tag")
COMPDEP_IMPL_END

 * These will then be displayed in the editor when selecting the component in a blueprint.
 */

#pragma region Helper Macros

// Marking it always protected makes sure child classes can call this when overriding
#define COMPDEP_DECL() \
protected: \
	virtual TArray<FComponentDependency> GetDependencies_Implementation() const override; \
private:


// Only conditionally have content in the implementation; the implementation itself is still needed though.
#if WITH_EDITOR
// Define old START macro so the change isn't breaking
#define COMPDEP_IMPL_START(ClassName) COMPDEP_IMPL_OVERRIDE_DEPS_BEGIN(ClassName)

#define COMPDEP_IMPL_OVERRIDE_DEPS_BEGIN(ClassName) \
TArray<FComponentDependency> ClassName::GetDependencies_Implementation() const { TArray<FComponentDependency> dependencies{};

#define COMPDEP_IMPL_EXTEND_DEPS_BEGIN(ClassName) \
TArray<FComponentDependency> ClassName::GetDependencies_Implementation() const { TArray<FComponentDependency> dependencies{ Super::GetDependencies_Implementation() };

#define COMPDEP_IMPL_END return dependencies; }

#define COMPDEP_DEP_AnyOnActorRequired(DependencyClass) \
dependencies.Add({ EComponentDependencyPosition::AnyOnActor, DependencyClass::StaticClass() });

#define COMPDEP_DEP_AnyOnActorOptional(DependencyClass) \
	dependencies.Add({ EComponentDependencyPosition::AnyOnActor, DependencyClass::StaticClass(), "", EComponentDependencyType::Optional });

#define COMPDEP_DEP_AnyOnActorWithTagRequired(DependencyClass, Tag) \
	dependencies.Add({ EComponentDependencyPosition::AnyOnActorWithTag, DependencyClass::StaticClass(), Tag });

#define COMPDEP_DEP_AnyOnActorWithTagOptional(DependencyClass, Tag) \
	dependencies.Add({ EComponentDependencyPosition::AnyOnActorWithTag, DependencyClass::StaticClass(), Tag, EComponentDependencyType::Optional });

#define COMPDEP_DEP_ChildRequired(DependencyClass) \
	dependencies.Add({ EComponentDependencyPosition::Child, DependencyClass::StaticClass() });

#define COMPDEP_DEP_ChildOptional(DependencyClass) \
	dependencies.Add({ EComponentDependencyPosition::Child, DependencyClass::StaticClass(), "", EComponentDependencyType::Optional });

#define COMPDEP_DEP_ChildWithTagRequired(DependencyClass, Tag) \
	dependencies.Add({ EComponentDependencyPosition::Child, DependencyClass::StaticClass(), Tag });

#define COMPDEP_DEP_ChildWithTagOptional(DependencyClass, Tag) \
	dependencies.Add({ EComponentDependencyPosition::ChildWithTag, DependencyClass::StaticClass(), Tag, EComponentDependencyType::Optional });


#else
#define COMPDEP_IMPL_START(ClassName) COMPDEP_IMPL_OVERRIDE_DEPS_BEGIN(ClassName)
#define COMPDEP_IMPL_OVERRIDE_DEPS_BEGIN(ClassName) TArray<FComponentDependency> ClassName::GetDependencies_Implementation() const { TArray<FComponentDependency> Deps;
#define COMPDEP_IMPL_EXTEND_DEPS_BEGIN(ClassName, SuperClass) TArray<FComponentDependency> ClassName::GetDependencies_Implementation() const { TArray<FComponentDependency> Deps;
#define COMPDEP_IMPL_END return Deps; }

#define COMPDEP_DEP_AnyOnActorRequired(DependencyClass)
#define COMPDEP_DEP_AnyOnActorOptional(DependencyClass)
#define COMPDEP_DEP_AnyOnActorWithTagRequired(DependencyClass, Tag)
#define COMPDEP_DEP_AnyOnActorWithTagOptional(DependencyClass, Tag)
#define COMPDEP_DEP_ChildRequired(DependencyClass)
#define COMPDEP_DEP_ChildOptional(DependencyClass)
#define COMPDEP_DEP_ChildWithTagRequired(DependencyClass, Tag)
#define COMPDEP_DEP_ChildWithTagOptional(DependencyClass, Tag)
#endif
#pragma endregion

UINTERFACE(MinimalAPI, Blueprintable)
class UComponentDependencies : public UInterface
{
    GENERATED_BODY()
};

class COMPDEP_API IComponentDependencies
{
    GENERATED_BODY()

public:
    // It'd be amazing if we could compile this only with WITH_EDITOR, but then blueprints break at runtime
    UFUNCTION(BlueprintNativeEvent)
    TArray<FComponentDependency> GetDependencies() const;
};
