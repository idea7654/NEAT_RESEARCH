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