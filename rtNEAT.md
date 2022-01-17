# rtNEAT

1. Calculate adjusted fitness

2. Remove the worst agent(Organism)

   - if the agent with the worst **unadjusted** fitness were chosen, fitness sharing could no longer protect innovation because new topologies would be removed as soon as they appear.

3. Re-estimating **F**(average fitness)

   - F needs to be calculated in each step

4. Creating offspring

   - the probability of choosing a given parent species is proportional to its average fitness compared to the total of all species' average fitness

5. Reassigning Agents to Species

   - In rtNEAT, changing threshold alone is not sufficient because of the population would still remain in their current species.

6. Replacing the old agent with the new one

   ​

## Problem

- If agents are replaced too frequently, they do not live enough to reach the minimum time **m** to be evaluated.

  ```javascript
  I = m / |P|n
  I - fraction of the population that is too young and therefore cannot be replaced.
  n - number of ticks betwwen replacement
  m - minimum time alive
  |P| - population size
  ```

- But, It is best to let the user choose I because in general it is most critical to performance. so,

  ```javascript
  n = m / |P|I
  ```

  ​

## Difference with original NEAT

### Original NEAT

- Measure fitness for all Organism
- Next Generation(Speciate, etc..)

### rtNEAT

- Create offspring one at a time, testing each offspring and replacing the worst with the new offspring if its better(Every tick)

1. frequency of compatibility threshold adjustment를 결정

2. 초기에 모든 population을 평가(organism이 있는지)

3. 각 species에서 모든 organism을 best-worst순으로 rank

4. 각 species의 평균 fitness 할당

5. 틱 시작

6. pop_size를 복제하고, num_species_target과 더 잘 일치하도록 compatibility threshold를 조정하고 새 species를 population에 재할당

7. ```cpp
   choose_parent_species() - 다음 offspring을 produce할 species를 결정
   reproduce_one() - 선택된 species의 front에 single offspring 생성
   ```

8. 7번을 통해 생긴 새 organism(offspring)을 평가.

9. 새 organism이 속한 species의 average fitness를 re-estimate

10. remove the worst organism

11. 5번으로 돌아가 반복

**즉! original NEAT는 Organism을 한번씩 돌면서 측정->다음 population 생성**

**rtNEAT는 틱마다 Population에서 worst를 지우고 새 Organism을 생성->여러 마리가 동시에 필요**



## Evaluate in Nero

1. To rtNEAT, Nero's sliders represent coefficients for fitness components.

2. Each individual fitness component is normalized to a Z-score(the number of standard deviations from the mean) so that each fitness component is measured on the same scale.

3. **Fitness** is computed as the sum of all these components multiplied by their slider levels, which can be positive or negative.(모든 컴포넌트의 합계에 슬라이더 레벨을 곱한 값으로 계산 )

   - Slider - NEAT가 optimize하는 fitness function의 해당 컴포넌트에 대한 계수를 지정하는데 사용,
   - static enemies, enemy turrets, rover, flags, and walls를 포함

4. 센서 type

   1. standard sensors - enemy radars, an "on target" sensor, object rangefinders, and line-of-fire sensors.

5. 각 Organism(Agent)는 fitness를 측정하는데 제한된 시간을 받음 -> 시간이 만료되면 factory로 돌아가 다른 evaluation을 시작 -> 신경망은 factory로 돌아간 agent만 대체함

6. Factory는 lucky 또는 unlucky를 얻을 수 없게 보장함. -> 모든 evaluation은 factory에서 일관되게 시작함

7. field에서 한 번 이상의 deployment로부터 살아남은 agent의 fitness는 과거일수록 deployment를 점차 잊어버리는 diminishing average를 통해 업데이트 됨

8. 실제 average는 처음 몇 번의 trials에 걸쳐 먼저 계산되고, 이후 연속 leaky average(TD(0) 강화학습 업데이트와 유사)가 유지됨

9. ```cpp
   f(t+1) = f(t) + (s(t) - f(t)) / r
   f(t) - current fitness
   s(t) - score from the current evaluation
   r - controls the rate of forgetting
   ```

10. 해당 process로 older agents는 younger agents보다 더 많은 deployment를 통해 평균화되기 때문에 더 신뢰할 수 있는 fitness measure을 갖고있지만, 그들의 fitness는 뒤떨어지지않는다.