<?php declare(strict_types=1);

namespace EtoA\Ship;

use Pimple\Container;
use Pimple\ServiceProviderInterface;

class ShipServiceProvider implements ServiceProviderInterface
{
    /**
     * {@inheritdoc}
     */
    public function register(Container $pimple): void
    {
        $pimple['etoa.ship.repository'] = function (Container $pimple) {
            return new ShipRepository($pimple['db']);
        };

        $pimple['etoa.ship.datarepository'] = function (Container $pimple) {
            return new ShipDataRepository($pimple['db'], $pimple['db.cache']);
        };
    }
}
